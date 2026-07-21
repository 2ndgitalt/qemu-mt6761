#!/bin/bash
# jules_setup.sh
# Prepares the Jules VM environment for QEMU compilation

set -e

echo "Updating package lists..."
sudo apt-get update

echo "Installing QEMU build dependencies..."
# Dependencies mirrored directly from the CI workflow
sudo apt-get install -y \
    ninja-build \
    python3 \
    python3-pip \
    libglib2.0-dev \
    libpixman-1-dev \
    flex \
    bison

echo "Configuring QEMU build environment..."
mkdir -p build
cd build

# Target only AArch64 to minimize compilation time and agent stalling
../configure --target-list=aarch64-softmmu --disable-docs --disable-tools

echo "Compiling QEMU..."
make -j$(nproc)

echo "Jules workspace setup complete. QEMU binary is ready at ./build/qemu-system-aarch64"
