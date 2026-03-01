/*
 * OpenOS - Driver Configuration (Rust FFI)
 *
 * C interface to the Rust driver_config static library.
 * The structs below mirror the #[repr(C)] types in
 * drivers/config/src/{console,keyboard,timer}_config.rs.
 *
 * Include this header to read or update driver settings at
 * runtime from C kernel code.
 */

#ifndef OPENOS_DRIVER_CONFIG_H
#define OPENOS_DRIVER_CONFIG_H

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* VGA color codes (matches VgaColor enum in console_config.rs)        */
/* ------------------------------------------------------------------ */
#define VGA_COLOR_BLACK        0
#define VGA_COLOR_BLUE         1
#define VGA_COLOR_GREEN        2
#define VGA_COLOR_CYAN         3
#define VGA_COLOR_RED          4
#define VGA_COLOR_MAGENTA      5
#define VGA_COLOR_BROWN        6
#define VGA_COLOR_LIGHT_GREY   7
#define VGA_COLOR_DARK_GREY    8
#define VGA_COLOR_LIGHT_BLUE   9
#define VGA_COLOR_LIGHT_GREEN  10
#define VGA_COLOR_LIGHT_CYAN   11
#define VGA_COLOR_LIGHT_RED    12
#define VGA_COLOR_PINK         13
#define VGA_COLOR_YELLOW       14
#define VGA_COLOR_WHITE        15

/* ------------------------------------------------------------------ */
/* Keyboard layout IDs (matches KeyboardLayout enum in                 */
/* keyboard_config.rs)                                                 */
/* ------------------------------------------------------------------ */
#define KB_LAYOUT_QWERTY_US    0
#define KB_LAYOUT_QWERTY_UK    1
#define KB_LAYOUT_AZERTY       2
#define KB_LAYOUT_QWERTZ       3

/* ------------------------------------------------------------------ */
/* Console configuration                                               */
/* ------------------------------------------------------------------ */

/**
 * ConsoleConfig - VGA text-mode console settings.
 *
 * Mirrors drivers/config/src/console_config.rs::ConsoleConfig.
 */
typedef struct {
    uint32_t width;        /**< Text columns (default 80).              */
    uint32_t height;       /**< Text rows    (default 25).              */
    uint8_t  fg_color;     /**< Foreground VGA color (default White=15).*/
    uint8_t  bg_color;     /**< Background VGA color (default Black=0). */
    uint8_t  auto_scroll;  /**< 1 = auto-scroll enabled (default 1).    */
    uint8_t  tab_width;    /**< Spaces per tab stop  (default 4).       */
} ConsoleConfig;

/** Copy the active console configuration into *cfg. */
void console_config_get(ConsoleConfig *cfg);
/** Replace the active console configuration with *cfg. */
void console_config_set(const ConsoleConfig *cfg);
/** Reset the console configuration to compiled-in defaults. */
void console_config_reset(void);
/** Write compiled-in defaults into *cfg without changing active config. */
void console_config_get_default(ConsoleConfig *cfg);

/* ------------------------------------------------------------------ */
/* Keyboard configuration                                              */
/* ------------------------------------------------------------------ */

/**
 * KeyboardConfig - PS/2 keyboard driver settings.
 *
 * Mirrors drivers/config/src/keyboard_config.rs::KeyboardConfig.
 */
typedef struct {
    uint8_t  layout;            /**< KB_LAYOUT_* value (default QWERTY_US=0). */
    uint32_t repeat_delay_ms;   /**< ms before key-repeat starts (default 500). */
    uint32_t repeat_rate_ms;    /**< ms between repeat events    (default 30).  */
    uint8_t  caps_lock_enabled; /**< 1 = caps-lock on at boot    (default 0).   */
    uint32_t buffer_size;       /**< Input ring-buffer capacity  (default 256). */
} KeyboardConfig;

/** Copy the active keyboard configuration into *cfg. */
void keyboard_config_get(KeyboardConfig *cfg);
/** Replace the active keyboard configuration with *cfg. */
void keyboard_config_set(const KeyboardConfig *cfg);
/** Reset the keyboard configuration to compiled-in defaults. */
void keyboard_config_reset(void);
/** Write compiled-in defaults into *cfg without changing active config. */
void keyboard_config_get_default(KeyboardConfig *cfg);

/* ------------------------------------------------------------------ */
/* Timer configuration                                                 */
/* ------------------------------------------------------------------ */

/**
 * TimerConfig - PIT (Programmable Interval Timer) settings.
 *
 * Mirrors drivers/config/src/timer_config.rs::TimerConfig.
 */
typedef struct {
    uint32_t frequency_hz;   /**< Interrupt frequency in Hz   (default 100). */
    uint32_t base_frequency; /**< PIT oscillator freq in Hz   (1193182 Hz).  */
    uint8_t  enabled;        /**< 1 = timer IRQ enabled       (default 1).   */
} TimerConfig;

/** Copy the active timer configuration into *cfg. */
void timer_config_get(TimerConfig *cfg);
/** Replace the active timer configuration with *cfg. */
void timer_config_set(const TimerConfig *cfg);
/** Reset the timer configuration to compiled-in defaults. */
void timer_config_reset(void);
/** Write compiled-in defaults into *cfg without changing active config. */
void timer_config_get_default(TimerConfig *cfg);

#endif /* OPENOS_DRIVER_CONFIG_H */
