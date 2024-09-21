CC ?= cc
CFLAGS := -std=gnu11 -O1 -g -Wall -Wextra -Iinclude -Ikmd

test: build/test_ring build/test_gpuva
	build/test_ring && build/test_gpuva

build/test_ring: tests/test_ring.c kmd/ring.c
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ $^

build/test_gpuva: tests/test_gpuva.c kmd/gpuva.c
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ $^
