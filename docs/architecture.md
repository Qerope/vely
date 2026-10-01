# Architecture

    app -> Vulkan loader -> UMD (vely_icd.so) --submit--> KMD ring --> GPU
                              |                              |
                              +--- buffer objects, VA -------+

- **UMD** (user mode driver): implements the Vulkan ICD entry points and later a
  Metal-facing layer. Builds command buffers in user memory; never touches
  hardware.
- **KMD** (kernel mode driver): owns the GPU virtual address space, validates
  and submits command buffers on a ring, signals fences.
- **First backend**: virtio-gpu. Real hardware backends come after the
  interfaces have survived a software one.

Rules we keep from the start: the UMD cannot name physical addresses; every
submission carries fence values, never pointers to kernel objects; the ring is
the only shared mutable state between UMD-visible submission and the KMD.

## Metal compatible layer (plan)

`MTLCommandBuffer` maps onto one KMD submission, an encoder maps to a range of
commands inside it, `MTLFence`/`MTLEvent` map onto timeline fences. Shaders
arrive as SPIR-V (through a translator), not as Metal Shading Language source.
