#include <assert.h>
#include <stdio.h>

#include "gpuva.h"

#define MB (1UL << 20)

int main(void)
{
    uint64_t a, b, c;
    gpuva_init(0x100000000UL, 64 * MB);
    assert(gpuva_alloc(MB, 4096, &a) == 0 && a == 0x100000000UL);
    assert(gpuva_alloc(MB, 4096, &b) == 0 && b == a + MB);
    assert(gpuva_alloc(MB, 4096, &c) == 0 && c == b + MB);
    assert(gpuva_free(b, MB) == 0);
    assert(gpuva_free(a, MB) == 0);             /* merges with the hole after it */
    assert(gpuva_free(c, MB) == 0);             /* merges both sides into one range */
    /* the whole 64MiB must be one range again, so a 64MiB allocation succeeds */
    assert(gpuva_alloc(64 * MB, 4096, &a) == 0 && a == 0x100000000UL);
    assert(gpuva_free(a, 64 * MB) == 0);
    assert(gpuva_alloc(MB, 2 * MB, &a) == 0 && a % (2 * MB) == 0);   /* alignment honoured */
    assert(gpuva_free(a, MB) == 0);
    printf("gpuva ok\n");
    return 0;
}
