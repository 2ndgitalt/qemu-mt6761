qemu-system-arm \
    -M virt \
    -cpu cortex-a15 \
    -smp 2 \
    -m 2048M \
    -kernel ./zImage \
    -dtb ./boot_a.bin_real.dtb \
    -append "console=ttyAMA0 earlycon=pl011,0x09000000 root=/dev/ram rdinit=/init loglevel=8" \
    -nographic \
    -s -S
