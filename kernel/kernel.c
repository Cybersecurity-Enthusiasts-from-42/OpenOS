/*
 * OpenOS - Kernel Main Entry Point
 * 
 * This is the main entry point for the OpenOS kernel after boot.
 * It initializes all kernel subsystems and enters the main kernel loop.
 */

#include "kernel.h"
#include "shell.h"
#include "../arch/x86/idt.h"
#include "../arch/x86/pic.h"
#include "../arch/x86/isr.h"
#include "../arch/x86/exceptions.h"
#include "../drivers/keyboard.h"
#include "../drivers/timer.h"
#include "../drivers/console.h"
#include "../fs/vfs.h"
/* #include "../memory/pmm.h" */  /* TODO: Uncomment when Multiboot info is passed */
/* #include "../memory/vmm.h" */  /* TODO: Uncomment when Multiboot info is passed */

/* Current working directory */
vfs_node_t* current_directory = 0;

/* Get current directory */
vfs_node_t* kernel_get_current_directory(void) {
    return current_directory;
}

/* Set current directory */
void kernel_set_current_directory(vfs_node_t* dir) {
    if (dir && dir->type == NODE_DIRECTORY) {
        current_directory = dir;
    }
}

/* Kernel entry point called from boot.S */
void kmain(void) {
    /* Initialize console */
    console_init();
    
    console_write("OpenOS - Advanced Educational Kernel\n");
    console_write("====================================\n");
    console_write("Running in 32-bit protected mode.\n\n");

    /* Initialize IDT */
    console_write("[1/6] Initializing IDT...\n");
    idt_init();
    
    /* Install exception handlers */
    console_write("[2/6] Installing exception handlers...\n");
    exceptions_init();
    
    /* Initialize PIC */
    console_write("[3/6] Initializing PIC...\n");
    pic_init();
    
    /* Initialize timer (100 Hz) */
    console_write("[4/6] Initializing timer...\n");
    timer_init(100);
    
    /* Install timer interrupt handler (IRQ0 = interrupt 0x20) */
    idt_set_gate(0x20, (uint32_t)irq0_handler, KERNEL_CODE_SEGMENT, IDT_FLAGS_KERNEL);
    
    /* Install keyboard interrupt handler (IRQ1 = interrupt 0x21) */
    console_write("[5/6] Initializing keyboard...\n");
    idt_set_gate(0x21, (uint32_t)irq1_handler, KERNEL_CODE_SEGMENT, IDT_FLAGS_KERNEL);
    
    /* Initialize keyboard (this will unmask IRQ1) */
    keyboard_init();
    
    /* Initialize VFS */
    console_write("[6/6] Initializing filesystem...\n");
    vfs_init();
    current_directory = vfs_get_root();
    
    /* TODO: When Multiboot info is passed to kmain(), uncomment these lines:
     * console_write("[7/8] Initializing physical memory...\n");
     * pmm_init(mboot);
     * 
     * console_write("[8/8] Initializing virtual memory...\n");
     * vmm_init();
     */
    
    /* Enable interrupts */
    __asm__ __volatile__("sti");
    
    /* Now that interrupts are enabled, unmask the timer IRQ */
    pic_unmask_irq(0);
    
    console_write("\n*** System Ready ***\n");
    console_write("- Exception handling: Active\n");
    console_write("- Timer interrupts: 100 Hz\n");
    console_write("- Keyboard: Ready\n");
    console_write("- Filesystem: Ready\n\n");
    console_write("Type 'help' for available commands.\n\n");
    
    /* Initialize shell */
    shell_init();
    
    /* Interactive shell loop */
    char input[256];
    while (1) {
        console_write("OpenOS> ");
        keyboard_get_line(input, sizeof(input));
        shell_execute(input);
    }
}
