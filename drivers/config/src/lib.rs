/*
 * OpenOS - Rust Driver Configuration Library
 *
 * Provides a no_std, bare-metal driver configuration framework.
 * Each driver module exposes configuration structs and C-callable
 * functions to get, set, and reset driver configuration at runtime.
 *
 * FFI structs use #[repr(C)] to guarantee ABI compatibility with
 * the kernel's C code.
 */

#![no_std]

pub mod console_config;
pub mod keyboard_config;
pub mod timer_config;

/// Bare-metal panic handler: halt execution on panic.
/// Required for no_std crates compiled as a staticlib.
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop {}
}
