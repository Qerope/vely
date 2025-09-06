#include "gpuva.h"

#include <stddef.h>

/* Sorted list of free ranges in a fixed pool: the kernel side must not call malloc. */
#define POOL 256

struct range {
    uint64_t start, size;
    struct range *next;
};

static struct range pool[POOL];
static struct range *free_list, *unused;

static struct range *node_get(void)
{
    struct range *r = unused;
    if (r) unused = r->next;
    return r;
}

static void node_put(struct range *r)
{
    r->next = unused;
    unused = r;
}

void gpuva_init(uint64_t base, uint64_t size)
{
    unused = NULL;
    for (int i = 0; i < POOL; i++) node_put(&pool[i]);
    free_list = node_get();
    free_list->start = base;
    free_list->size = size;
    free_list->next = NULL;
}

/* align must be a power of two. The padding in front of an aligned address stays free. */
int gpuva_alloc(uint64_t size, uint64_t align, uint64_t *out)
{
    for (struct range **pp = &free_list; *pp; pp = &(*pp)->next) {
        struct range *r = *pp;
        uint64_t start = (r->start + align - 1) & ~(align - 1);
        if (start < r->start || start - r->start > r->size || r->size - (start - r->start) < size) continue;
        uint64_t pad = start - r->start;
        uint64_t tail = r->size - pad - size;
        if (pad && tail) {
            struct range *t = node_get();
            if (!t) return -1;
            t->start = start + size;
            t->size = tail;
            t->next = r->next;
            r->next = t;
            r->size = pad;
        } else if (pad) {
            r->size = pad;
        } else if (tail) {
            r->start = start + size;
            r->size = tail;
        } else {
            *pp = r->next;
            node_put(r);
        }
        *out = start;
        return 0;
    }
    return -1;
}

int gpuva_free(uint64_t addr, uint64_t size)
{
    struct range **pp = &free_list;
    while (*pp && (*pp)->start < addr) pp = &(*pp)->next;
    struct range *prev = (pp == &free_list) ? NULL : (struct range *)((char *)pp - offsetof(struct range, next));
    struct range *next = *pp;
    if (prev && prev->start + prev->size > addr) return -1;       /* overlaps a free range */
    if (next && addr + size > next->start) return -1;
    if (prev && prev->start + prev->size == addr) {
        prev->size += size;
        if (next && prev->start + prev->size == next->start) {
            prev->size += next->size;
            prev->next = next->next;
            node_put(next);
        }
        return 0;
    }
    if (next && addr + size == next->start) {
        next->start = addr;
        next->size += size;
        return 0;
    }
    struct range *r = node_get();
    if (!r) return -1;
    r->start = addr;
    r->size = size;
    r->next = *pp;
    *pp = r;
    return 0;
}
