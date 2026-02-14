#include "ring.h"

int ring_push(struct ring *r, const struct vely_cmd *c)
{
    uint32_t head = r->head;
    if (head - __atomic_load_n(&r->tail, __ATOMIC_ACQUIRE) == RING_SIZE) return -1;  /* full */
    r->slots[head & (RING_SIZE - 1)] = *c;
    __atomic_store_n(&r->head, head + 1, __ATOMIC_RELEASE);
    return 0;
}

int ring_pop(struct ring *r, struct vely_cmd *out)
{
    uint32_t tail = r->tail;
    if (tail == __atomic_load_n(&r->head, __ATOMIC_ACQUIRE)) return -1;
    *out = r->slots[tail & (RING_SIZE - 1)];
    __atomic_store_n(&r->tail, tail + 1, __ATOMIC_RELEASE);
    return 0;
}
