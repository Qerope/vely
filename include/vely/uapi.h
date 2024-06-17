#pragma once
#include <stdint.h>

/* Shared between UMD and KMD. Fixed-width, no pointers inside. */

struct vely_bo_create {
    uint64_t size;
    uint32_t flags;
    uint32_t handle;   /* out */
};

struct vely_bo_map_gpu {
    uint32_t handle;
    uint32_t align_log2;
    uint64_t gpu_va;   /* out */
};

struct vely_submit {
    uint64_t cmds_handle;   /* BO holding the command stream */
    uint32_t cmds_offset;
    uint32_t cmds_size;
    uint64_t wait_fence;    /* timeline value to wait for, 0 = none */
    uint64_t signal_fence;  /* timeline value to signal on completion */
};
