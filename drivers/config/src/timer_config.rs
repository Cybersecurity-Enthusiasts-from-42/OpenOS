/*
 * OpenOS - Timer Driver Configuration
 *
 * Manages the Programmable Interval Timer (PIT) settings:
 * interrupt frequency and enabled state.
 */

/// Timer driver configuration passed across the C FFI boundary.
///
/// Layout is identical to the C `TimerConfig` struct in
/// `drivers/driver_config.h`.
#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct TimerConfig {
    /// Desired timer interrupt frequency in Hz (default 100 Hz).
    pub frequency_hz: u32,
    /// PIT base oscillator frequency in Hz (fixed hardware value: 1 193 182 Hz).
    pub base_frequency: u32,
    /// Non-zero to enable the timer interrupt (default 1).
    pub enabled: u8,
}

impl TimerConfig {
    /// Default configuration: 100 Hz interrupts, timer enabled.
    pub const fn default() -> Self {
        TimerConfig {
            frequency_hz: 100,
            base_frequency: 1193182,
            enabled: 1,
        }
    }
}

/// Global timer configuration state.
///
/// # Concurrency
/// This static is intentionally `static mut`.  The kernel configures
/// drivers during single-threaded boot, before any interrupt or
/// secondary-CPU activity.  Callers that invoke these functions after
/// boot (e.g., from an interrupt handler) are responsible for
/// ensuring mutual exclusion (e.g., by disabling interrupts).
static mut TIMER_CONFIG: TimerConfig = TimerConfig::default();

/// Copy the current timer configuration into `*cfg`.
/// Does nothing if `cfg` is NULL.
///
/// # Safety
/// The caller must ensure that no concurrent access to the global
/// timer configuration occurs (e.g., disable interrupts around the
/// call when invoked outside of the single-threaded boot sequence).
#[no_mangle]
pub unsafe extern "C" fn timer_config_get(cfg: *mut TimerConfig) {
    if !cfg.is_null() {
        *cfg = TIMER_CONFIG;
    }
}

/// Replace the current timer configuration with `*cfg`.
/// Does nothing if `cfg` is NULL.
///
/// # Safety
/// Same as [`timer_config_get`]: the caller must guarantee exclusive
/// access to the global timer configuration.
#[no_mangle]
pub unsafe extern "C" fn timer_config_set(cfg: *const TimerConfig) {
    if !cfg.is_null() {
        TIMER_CONFIG = *cfg;
    }
}

/// Reset the timer configuration to the compiled-in defaults.
///
/// # Safety
/// Same as [`timer_config_get`].
#[no_mangle]
pub unsafe extern "C" fn timer_config_reset() {
    TIMER_CONFIG = TimerConfig::default();
}

/// Write the compiled-in default configuration into `*cfg`
/// without modifying the active configuration.
/// Does nothing if `cfg` is NULL.
#[no_mangle]
pub extern "C" fn timer_config_get_default(cfg: *mut TimerConfig) {
    if !cfg.is_null() {
        unsafe {
            *cfg = TimerConfig::default();
        }
    }
}
