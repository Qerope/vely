#pragma once
#include <stdint.h>

void gpuva_init(uint64_t base, uint64_t size);
int gpuva_alloc(uint64_t size, uint64_t align, uint64_t *out);
int gpuva_free(uint64_t addr, uint64_t size);
