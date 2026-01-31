/*
 * OpenOS - Step 0
 *
 * Minimal freestanding kernel entry.
 * Runs in 32-bit protected mode, writes to VGA text buffer,
 * and then halts the CPU in a loop.
 */

#include <stdint.h>
#include <stddef.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((uint16_t*)0xB8000)

static uint16_t* const vga_buf = VGA_MEMORY;
static size_t term_row = 0;
static size_t term_col = 0;
static uint8_t term_color = 0x0F; /* white on black */

static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)(uint8_t)c | ((uint16_t)color << 8);
}

static void terminal_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buf[y * VGA_WIDTH + x] = vga_entry(' ', term_color);
        }
    }
    term_row = 0;
    term_col = 0;
}

static void terminal_scroll(void) {
    /* Scroll up by moving all lines up by one */
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buf[y * VGA_WIDTH + x] = vga_buf[(y + 1) * VGA_WIDTH + x];
        }
    }
    /* Clear the last line */
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        vga_buf[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', term_color);
    }
}

static void terminal_put_char(char c) {
    if (c == '\n') {
        term_col = 0;
        term_row++;
        if (term_row >= VGA_HEIGHT) {
            terminal_scroll();
            term_row = VGA_HEIGHT - 1;
        }
        return;
    }

    vga_buf[term_row * VGA_WIDTH + term_col] = vga_entry(c, term_color);
    term_col++;
    if (term_col >= VGA_WIDTH) {
        term_col = 0;
        term_row++;
        if (term_row >= VGA_HEIGHT) {
            terminal_scroll();
            term_row = VGA_HEIGHT - 1;
        }
    }
}

static void terminal_write(const char* s) {
    for (size_t i = 0; s[i] != '\0'; i++) {
        terminal_put_char(s[i]);
    }
}

/* Kernel entry point called from boot.S */
void kmain(void) {
    terminal_clear();
    terminal_write("OpenOS - Educational Kernel Prototype\n");
    terminal_write("-------------------------------------\n");
    terminal_write("Running in 32-bit protected mode.\n");
    terminal_write("Next steps: GDT/IDT, interrupts, paging, processes...\n");

    /* Halt the CPU forever */
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
