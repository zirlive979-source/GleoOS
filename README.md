# GleoOS v1.0.1

**Developer:** Dika  
**Type:** Educational mini operating system  
**Initial target:** x86 PC / QEMU / VirtualBox  
**Boot:** GRUB Multiboot → GleoOS kernel

## Current v1.0.1 prototype

- Custom GleoOS boot splash
- 32-bit protected-mode kernel entry
- VGA text-mode desktop shell
- Keyboard input
- Simple commands: help, clear, about, reboot
- GleoOS branding
- Privacy & Policy and License documents

## Important scope

This is a real bootable-kernel prototype, not a complete commercial OS yet.
Chrome/Google Play Store are not bundled. They require their own supported platform,
licensing, and/or services. Future GleoOS versions can provide a browser and an
application store designed for GleoOS.

## Build requirements

A Linux development environment with:
- GNU make
- GCC with 32-bit support (`gcc -m32`)
- NASM
- GRUB tools (`grub-mkrescue`)
- xorriso
- QEMU (optional, for testing)

On Debian/Ubuntu, packages commonly used for this prototype are:

    sudo apt update
    sudo apt install build-essential gcc-multilib nasm grub-pc-bin xorriso qemu-system-x86

## Build

    make

This creates:

    build/gleoos-v1.0.1.iso

## Run with QEMU

    make run

Or:

    qemu-system-i386 -cdrom build/gleoos-v1.0.1.iso

## Project layout

    boot/       Multiboot entry
    kernel/     C kernel and VGA/keyboard code
    linker.ld   Kernel linker script
    grub.cfg    GRUB configuration
    Makefile    Build system
    docs/       Privacy and license
