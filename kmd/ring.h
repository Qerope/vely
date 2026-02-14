#pragma once
#include <stdint.h>

#define RING_SIZE 256   /* must be a power of two */

struct vely_cmd {
    uint32_t op;
    uint32_t arg[7];
};

struct ring {
    struct vely_cmd slots[RING_SIZE];
    uint32_t head, tail;   /* free-running; wrap by unsigned overflow, index with & (RING_SIZE - 1) */
};

int ring_push(struct ring *r, const struct vely_cmd *c);
int ring_pop(struct ring *r, struct vely_cmd *out);
