/*
 * OpenOS - Graphics and Windowing System
 * 
 * Provides basic GUI framework with framebuffer support
 */

#ifndef OPENOS_GUI_H
#define OPENOS_GUI_H

#include <stdint.h>

/* Screen dimensions */
#define GUI_WIDTH 800
#define GUI_HEIGHT 600
#define GUI_BPP 32  /* Bits per pixel */

/* Color definitions (ARGB format) */
#define COLOR_BLACK   0xFF000000
#define COLOR_WHITE   0xFFFFFFFF
#define COLOR_RED     0xFFFF0000
#define COLOR_GREEN   0xFF00FF00
#define COLOR_BLUE    0xFF0000FF
#define COLOR_GRAY    0xFF808080
#define COLOR_LIGHTGRAY 0xFFC0C0C0

/* Window states */
typedef enum {
    WINDOW_HIDDEN,
    WINDOW_VISIBLE,
    WINDOW_MINIMIZED
} window_state_t;

/* Rectangle structure */
typedef struct rect {
    int x, y;
    int width, height;
} rect_t;

/* Window structure */
typedef struct window {
    uint32_t id;
    char title[64];
    rect_t rect;
    uint32_t bg_color;
    window_state_t state;
    struct window* next;
} window_t;

/* GUI system state */
typedef struct gui_state {
    uint32_t* framebuffer;
    int width, height;
    window_t* window_list;
    uint32_t next_window_id;
    int initialized;
} gui_state_t;

/* Initialize GUI subsystem */
void gui_init(void);

/* Drawing primitives */
void gui_draw_pixel(int x, int y, uint32_t color);
void gui_draw_rect(rect_t* rect, uint32_t color);
void gui_draw_filled_rect(rect_t* rect, uint32_t color);
void gui_draw_line(int x1, int y1, int x2, int y2, uint32_t color);
void gui_draw_text(int x, int y, const char* text, uint32_t color);

/* Window management */
window_t* gui_create_window(int x, int y, int width, int height, const char* title);
void gui_destroy_window(window_t* window);
void gui_show_window(window_t* window);
void gui_hide_window(window_t* window);
void gui_render_window(window_t* window);
void gui_render_all(void);

/* Clear screen */
void gui_clear_screen(uint32_t color);

#endif /* OPENOS_GUI_H */
