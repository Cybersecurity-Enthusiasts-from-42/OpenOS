# Debugging OpenOS with QEMU and GDB

This guide explains how to debug the OpenOS kernel using QEMU's built-in GDB
stub together with the GNU Debugger (`gdb`).

## Prerequisites

| Tool | Install (Ubuntu/Debian) | Install (Arch Linux) |
|------|------------------------|----------------------|
| `qemu-system-i386` | `sudo apt-get install qemu-system-x86` | `sudo pacman -S qemu-system-x86` |
| `gdb` | `sudo apt-get install gdb` | `sudo pacman -S gdb` |

## Quick Start

### 1. Start QEMU in debug mode

In one terminal run:

```bash
make debug
```

This builds the bootable ISO if necessary, then starts QEMU with:

- **`-s`** – opens a GDB server on `tcp::1234`
- **`-S`** – pauses CPU execution at startup (QEMU waits for GDB `continue`)
- **`-serial mon:stdio`** – routes serial output and the QEMU monitor to the
  current terminal so you can read kernel log messages

QEMU will appear frozen until a GDB client connects and issues `continue`.

### 2. Attach GDB

In a **second terminal** run:

```bash
make gdb
```

This launches `gdb`, loads the kernel binary for symbol information, and
connects to the QEMU GDB server automatically. If the kernel binary is not
found you will see a note asking you to run `make` first.

You can also attach manually:

```bash
gdb
(gdb) file Kernel2.0/openos.bin   # load symbols (optional but recommended)
(gdb) target remote :1234         # connect to QEMU
```

### 3. Start execution

```
(gdb) continue
```

QEMU will now start booting OpenOS and serial output will appear in the first
terminal.

## Useful GDB Commands

| Command | Description |
|---------|-------------|
| `continue` (or `c`) | Resume execution |
| `Ctrl+C` | Interrupt running kernel |
| `info registers` | Display CPU register values |
| `x/10i $eip` | Disassemble 10 instructions at the current instruction pointer |
| `x/20wx 0x100000` | Examine 20 words of memory at address 0x100000 |
| `break *0x<addr>` | Set a hardware breakpoint at a physical address |
| `stepi` (or `si`) | Step one machine instruction |
| `nexti` (or `ni`) | Step one machine instruction, stepping over calls |
| `bt` | Print backtrace (requires symbol info) |
| `info threads` | List QEMU vCPUs as GDB threads |
| `disconnect` | Detach from QEMU without stopping it |
| `quit` (or `q`) | Exit GDB |

> **Tip:** Build the kernel with debug symbols by passing `CFLAGS+=-g` to
> `make` (e.g. `make CFLAGS+=-g`). This enables source-level debugging so you
> can use `break kernel/kernel.c:42` style breakpoints and `list` to view
> source code in GDB.

## QEMU Logging

To capture detailed QEMU event logs (interrupts, CPU resets, guest errors) to
a file while running the kernel normally:

```bash
make qemu-log
```

The log is written to `qemu.log` in the repository root. You can inspect it
with any text editor or `less qemu.log`.

To change the log categories, edit the `qemu-log` target in the `Makefile` and
modify the `-d` flag. See `qemu-system-i386 -d help` for a full list of
available log categories.

## Normal Run (no debugging)

The existing `make run` and `make run-iso` targets are unchanged and start
QEMU without a GDB server or serial redirection:

```bash
make run       # build kernel + ISO, then boot in QEMU
make run-iso   # build ISO only, then boot in QEMU
```

## Workflow Summary

```
Terminal 1                        Terminal 2
──────────────────────────────    ──────────────────────────────
$ make debug                      (wait for QEMU to start)
[QEMU paused, waiting for GDB]    $ make gdb
                                  (gdb) break kernel_main
                                  (gdb) continue
[serial output appears here]      [hit breakpoint in kernel_main]
                                  (gdb) info registers
                                  (gdb) continue
```
