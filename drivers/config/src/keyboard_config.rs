/*
 * OpenOS - Keyboard Driver Configuration
 *
 * Manages keyboard settings: layout selection, key-repeat timing,
 * initial caps-lock state, and input buffer capacity.
 */

/// Supported keyboard layouts.
#[repr(u8)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum KeyboardLayout {
    QwertyUs = 0,
    QwertyUk = 1,
    Azerty = 2,
    Qwertz = 3,
}

/// Keyboard driver configuration passed across the C FFI boundary.
///
/// Layout is identical to the C `KeyboardConfig` struct in
/// `drivers/driver_config.h`.
#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct KeyboardConfig {
    /// Active layout (KeyboardLayout value, default QwertyUs = 0).
    pub layout: u8,
    /// Milliseconds before key-repeat begins (default 500 ms).
    pub repeat_delay_ms: u32,
    /// Milliseconds between successive key-repeat events (default 30 ms).
    pub repeat_rate_ms: u32,
    /// Non-zero if caps-lock is initially active (default 0).
    pub caps_lock_enabled: u8,
    /// Capacity of the keyboard input ring buffer in bytes (default 256).
    pub buffer_size: u32,
}

impl KeyboardConfig {
    /// Default configuration matching typical BIOS/firmware defaults.
    pub const fn default() -> Self {
        KeyboardConfig {
            layout: KeyboardLayout::QwertyUs as u8,
            repeat_delay_ms: 500,
            repeat_rate_ms: 30,
            caps_lock_enabled: 0,
            buffer_size: 256,
        }
    }
}

/// Global keyboard configuration state.
///
/// # Concurrency
/// This static is intentionally `static mut`.  The kernel configures
/// drivers during single-threaded boot, before any interrupt or
/// secondary-CPU activity.  Callers that invoke these functions after
/// boot (e.g., from an interrupt handler) are responsible for
/// ensuring mutual exclusion (e.g., by disabling interrupts).
static mut KEYBOARD_CONFIG: KeyboardConfig = KeyboardConfig::default();

/// Copy the current keyboard configuration into `*cfg`.
/// Does nothing if `cfg` is NULL.
///
/// # Safety
/// The caller must ensure that no concurrent access to the global
/// keyboard configuration occurs (e.g., disable interrupts around the
/// call when invoked outside of the single-threaded boot sequence).
#[no_mangle]
pub unsafe extern "C" fn keyboard_config_get(cfg: *mut KeyboardConfig) {
    if !cfg.is_null() {
        *cfg = KEYBOARD_CONFIG;
    }
}

/// Replace the current keyboard configuration with `*cfg`.
/// Does nothing if `cfg` is NULL.
///
/// # Safety
/// Same as [`keyboard_config_get`]: the caller must guarantee exclusive
/// access to the global keyboard configuration.
#[no_mangle]
pub unsafe extern "C" fn keyboard_config_set(cfg: *const KeyboardConfig) {
    if !cfg.is_null() {
        KEYBOARD_CONFIG = *cfg;
    }
}

/// Reset the keyboard configuration to the compiled-in defaults.
///
/// # Safety
/// Same as [`keyboard_config_get`].
#[no_mangle]
pub unsafe extern "C" fn keyboard_config_reset() {
    KEYBOARD_CONFIG = KeyboardConfig::default();
}

/// Write the compiled-in default configuration into `*cfg`
/// without modifying the active configuration.
/// Does nothing if `cfg` is NULL.
#[no_mangle]
pub extern "C" fn keyboard_config_get_default(cfg: *mut KeyboardConfig) {
    if !cfg.is_null() {
        unsafe {
            *cfg = KeyboardConfig::default();
        }
    }
}
