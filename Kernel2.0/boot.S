/*
 * OpenOS - minimal multiboot kernel loader
 * Boots via GRUB in 32-bit protected mode and calls kmain().
 */

.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
    .align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.section .text
    .global _start
    .extern kmain

_start:
    cli

    /* Set up a simple stack */
    mov $stack_top, %esp

    /* Call C kernel entry */
    call kmain

.Lhalt:
    cli
    hlt
    jmp .Lhalt

.section .bss
    .align 16
stack_bottom:
    .space 16384       /* 16 KiB stack */
stack_top:
