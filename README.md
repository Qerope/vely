# VEly

Graphics driver stack for Elysium. Very early: there is no real GPU backend yet.
The first backend is virtio-gpu under QEMU, because it lets the kernel/user
split be exercised without hardware.
