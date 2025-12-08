# OpenOS – Minimal Educational Kernel (Step 0)

This is the **first bootstrap step** of **OpenOS**, a small educational
operating system kernel designed to help you learn how kernels work from
the ground up.

This step gives you:

- A **Multiboot-compatible** kernel entry (`boot.S`) for GRUB
- A simple **32‑bit protected mode** C `kmain()`
- **VGA text mode** output to the screen
- A minimal **Makefile** to build and run the kernel with QEMU

The kernel boots, clears the screen, prints a short banner, and then halts
the CPU in an infinite loop.

---

## Prerequisites

You will need:

- A cross‑compiler (or native) toolchain that can target bare‑metal i386:
  - `i686-elf-gcc`
  - `i686-elf-binutils` (for `ld`, etc., if you prefer not to use GCC as the linker)
- An emulator, e.g.:
  - `qemu-system-i386`

On many Linux distros you can build your own `i686-elf-gcc` following
standard OSDev.org tutorials.

---

## Build

From the project root:

```bash
make
```

This produces:

- `openos.bin` – the kernel binary suitable for Multiboot (e.g. GRUB / QEMU)

---

## Run with QEMU (direct kernel loading)

```bash
make run
```

This is equivalent to:

```bash
qemu-system-i386 -kernel openos.bin
```

QEMU will boot directly into the kernel (as if loaded by a Multiboot
bootloader). You should see a text screen like:

```text
OpenOS - Educational Kernel Prototype
-------------------------------------
Running in 32-bit protected mode.
Next steps: GDT/IDT, interrupts, paging, processes...
```

---

## Next Steps (Roadmap)

Future steps for OpenOS (that we can implement incrementally):

1. **CPU Setup**
   - Proper GDT setup
   - IDT and basic exception handlers (e.g. page fault, general protection)

2. **Interrupts & Timer**
   - PIC remapping
   - PIT (timer) initialization
   - A global tick counter and simple heartbeat message

3. **Memory Management**
   - Physical memory bitmap allocator
   - Basic paging (identity map + simple higher-half kernel)

4. **Processes & Scheduler**
   - `struct process` with register state
   - Context switch routine
   - Round‑robin scheduler driven by the timer interrupt

5. **Syscalls & User Space**
   - Simple syscall interface
   - Minimal userland program loader

Each of these can be added in small, understandable commits on top of this
Step 0 kernel.

---

## License

You can license this however you like in your own repo. For now, consider
this initial scaffold MIT‑style: do whatever you want with it, with no
warranty of any kind.
