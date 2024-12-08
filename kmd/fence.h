#pragma once
#include <stdint.h>

/* Timeline fence: a 64 bit counter that only moves forward. Waiters ask for a value. */
struct fence {
    uint64_t signaled;
};

static inline int fence_reached(const struct fence *f, uint64_t value)
{
    return __atomic_load_n(&f->signaled, __ATOMIC_ACQUIRE) >= value;
}

/* Returns -1 if value would move the fence backwards. */
static inline int fence_signal(struct fence *f, uint64_t value)
{
    uint64_t cur = __atomic_load_n(&f->signaled, __ATOMIC_RELAXED);
    while (value > cur) {
        if (__atomic_compare_exchange_n(&f->signaled, &cur, value, 0, __ATOMIC_RELEASE, __ATOMIC_RELAXED))
            return 0;
    }
    return value == cur ? 0 : -1;
}
