/*
 * OpenOS - Console Driver Configuration
 *
 * Manages VGA text-mode console settings: dimensions, colors,
 * auto-scroll, and tab width.  All state is held in a single
 * static variable so no heap is required.
 */

/// VGA 16-color palette codes (matches hardware register values).
#[repr(u8)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum VgaColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGrey = 7,
    DarkGrey = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    Pink = 13,
    Yellow = 14,
    White = 15,
}

/// Console driver configuration passed across the C FFI boundary.
///
/// Layout is identical to the C `ConsoleConfig` struct in
/// `drivers/driver_config.h`.
#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct ConsoleConfig {
    /// Number of text columns (default 80).
    pub width: u32,
    /// Number of text rows (default 25).
    pub height: u32,
    /// Foreground color index (VGA palette, default White = 15).
    pub fg_color: u8,
    /// Background color index (VGA palette, default Black = 0).
    pub bg_color: u8,
    /// Non-zero to enable automatic scrolling when the cursor
    /// reaches the last row (default 1).
    pub auto_scroll: u8,
    /// Number of spaces per tab stop (default 4).
    pub tab_width: u8,
}

impl ConsoleConfig {
    /// Default configuration matching the hardware reset state.
    pub const fn default() -> Self {
        ConsoleConfig {
            width: 80,
            height: 25,
            fg_color: VgaColor::White as u8,
            bg_color: VgaColor::Black as u8,
            auto_scroll: 1,
            tab_width: 4,
        }
    }
}

/// Global console configuration state.
///
/// # Concurrency
/// This static is intentionally `static mut`.  The kernel configures
/// drivers during single-threaded boot, before any interrupt or
/// secondary-CPU activity.  Callers that invoke these functions after
/// boot (e.g., from an interrupt handler) are responsible for
/// ensuring mutual exclusion (e.g., by disabling interrupts).
static mut CONSOLE_CONFIG: ConsoleConfig = ConsoleConfig::default();

/// Copy the current console configuration into `*cfg`.
/// Does nothing if `cfg` is NULL.
///
/// # Safety
/// The caller must ensure that no concurrent access to the global
/// console configuration occurs (e.g., disable interrupts around the
/// call when invoked outside of the single-threaded boot sequence).
#[no_mangle]
pub unsafe extern "C" fn console_config_get(cfg: *mut ConsoleConfig) {
    if !cfg.is_null() {
        *cfg = CONSOLE_CONFIG;
    }
}

/// Replace the current console configuration with `*cfg`.
/// Does nothing if `cfg` is NULL.
///
/// # Safety
/// Same as [`console_config_get`]: the caller must guarantee exclusive
/// access to the global console configuration.
#[no_mangle]
pub unsafe extern "C" fn console_config_set(cfg: *const ConsoleConfig) {
    if !cfg.is_null() {
        CONSOLE_CONFIG = *cfg;
    }
}

/// Reset the console configuration to the compiled-in defaults.
///
/// # Safety
/// Same as [`console_config_get`].
#[no_mangle]
pub unsafe extern "C" fn console_config_reset() {
    CONSOLE_CONFIG = ConsoleConfig::default();
}

/// Write the compiled-in default configuration into `*cfg`
/// without modifying the active configuration.
/// Does nothing if `cfg` is NULL.
#[no_mangle]
pub extern "C" fn console_config_get_default(cfg: *mut ConsoleConfig) {
    if !cfg.is_null() {
        unsafe {
            *cfg = ConsoleConfig::default();
        }
    }
}
