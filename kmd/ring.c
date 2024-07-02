#include "ring.h"

int ring_push(struct ring *r, const struct vely_cmd *c)
{
    uint32_t head = r->head;
    uint32_t next = (head + 1) % RING_SIZE;
    if (next == __atomic_load_n(&r->tail, __ATOMIC_ACQUIRE)) return -1;  /* full (wastes one slot) */
    r->slots[head] = *c;
    __atomic_store_n(&r->head, next, __ATOMIC_RELEASE);  /* slot contents visible before head moves */
    return 0;
}

int ring_pop(struct ring *r, struct vely_cmd *out)
{
    uint32_t tail = r->tail;
    if (tail == __atomic_load_n(&r->head, __ATOMIC_ACQUIRE)) return -1;
    *out = r->slots[tail];
    __atomic_store_n(&r->tail, (tail + 1) % RING_SIZE, __ATOMIC_RELEASE);
    return 0;
}
