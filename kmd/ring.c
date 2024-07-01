#include "ring.h"

int ring_push(struct ring *r, const struct vely_cmd *c)
{
    uint32_t next = (r->head + 1) % RING_SIZE;
    if (next == r->tail) return -1;  /* full (wastes one slot) */
    r->slots[r->head] = *c;
    r->head = next;
    return 0;
}

int ring_pop(struct ring *r, struct vely_cmd *out)
{
    if (r->head == r->tail) return -1;
    *out = r->slots[r->tail];
    r->tail = (r->tail + 1) % RING_SIZE;
    return 0;
}
