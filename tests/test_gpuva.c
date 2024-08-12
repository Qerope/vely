    assert(gpuva_free(a, MB) == 0);             /* merges with the hole after it */
    assert(gpuva_free(c, MB) == 0);             /* merges both sides into one range */
