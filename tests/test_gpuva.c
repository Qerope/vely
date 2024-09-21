#include <assert.h>
#include <stdio.h>

#include "gpuva.h"

#define MB (1UL << 20)

int main(void)
{
    uint64_t a, b, c;
    gpuva_init(0x100000000UL, 64 * MB);
    assert(gpuva_alloc(MB, &a) == 0 && a == 0x100000000UL);
    assert(gpuva_alloc(MB, &b) == 0 && b == a + MB);
    assert(gpuva_alloc(MB, &c) == 0 && c == b + MB);
    assert(gpuva_free(b, MB) == 0);
    assert(gpuva_free(a, MB) == 0);             /* merges with the hole after it */
    assert(gpuva_free(c, MB) == 0);             /* merges both sides into one range */
    printf("gpuva ok\n");
    return 0;
}
