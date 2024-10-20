#include "kernel/kernel.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/timer/timer.h"
#include "drivers/serial_port/serial_port.h"
#include "bash/bash.h"
#include "bash/messages/messages.h"
#include "drivers/timer_handler/timer_handler.h"

void exception_handler(u32 interrupt, u32 error, char *message) {
    serial_log(LOG_ERROR, message);
}

void init_kernel() {
    init_gdt();
    init_idt();
    init_exception_handlers();
    init_interrupt_handlers();
    register_timer_interrupt_handler();
    register_keyboard_interrupt_handler();
    configure_default_serial_port();
    set_exception_handler(exception_handler);
    enable_interrupts();
}

_Noreturn void halt_loop() {
    while (1) { halt(); }
}

/**
 * This is where the bootloader transfers control to.
 */
void kernel_entry() {
    init_kernel();
    init_bash();
    timer_set_handler(main_timer_handler);
    halt_loop();
}
