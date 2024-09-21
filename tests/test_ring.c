#include <assert.h>
#include <pthread.h>
#include <stdio.h>

#include "ring.h"

static struct ring r;
#define N 200000

static void *producer(void *arg)
{
    (void)arg;
    for (uint32_t i = 0; i < N;) {
        struct vely_cmd c = {.op = i};
        if (ring_push(&r, &c) == 0) i++;
    }
    return NULL;
}

int main(void)
{
    struct vely_cmd c;
    assert(ring_pop(&r, &c) == -1);
    for (int i = 0; i < RING_SIZE - 1; i++) {
        c.op = i;
        assert(ring_push(&r, &c) == 0);
    }
    assert(ring_push(&r, &c) == -1);
    for (uint32_t i = 0; i < RING_SIZE - 1; i++) {
        assert(ring_pop(&r, &c) == 0);
        assert(c.op == i);
    }

    /* two threads: every command arrives once, in order */
    pthread_t t;
    pthread_create(&t, NULL, producer, NULL);
    for (uint32_t i = 0; i < N;) {
        if (ring_pop(&r, &c) == 0) {
            assert(c.op == i);
            i++;
        }
    }
    pthread_join(t, NULL);
    printf("ring ok\n");
    return 0;
}
