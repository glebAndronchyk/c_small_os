#include "../../drivers/framebuffer/framebuffer.h";
#include "../../utils/VGA_COLORS/VGA_COLORS.h";
#include "../messages/messages.h";

void app_name() {
    struct message_config no_starter_cfg = {
        .bg = COLOR_GREEN,
        .fg = COLOR_WHITE,
        .starter = &"",
    };

    struct message_config starter_cfg = {
        .bg = COLOR_GREEN,
        .fg = COLOR_WHITE,
        .starter = &"> ",
    };

    system_message("                 ", no_starter_cfg);
    system_message("   Bash v0.0.1   ", no_starter_cfg);
    system_message("                 ", starter_cfg);
}

void char_press(char* msg) {
    write_buffer_message(msg, DEFAULT_BACKGROUND, DEFAULT_FOREGROUND);
}

void execution_error(char* msg) {
    system_message(msg, (struct message_config){
        .bg = COLOR_RED,
        .fg = COLOR_WHITE,
        .starter = &"> ",
        .new_lines_offset = 1,
    });
}

void execution_success_sequentially(char* msg) {
    system_message(msg, (struct message_config){
        .bg = DEFAULT_BACKGROUND,
        .fg = COLOR_GREEN,
       .starter = &"",
       .new_lines_offset = 0,
   });
}

void execution_success(char* msg) {
    system_message(msg, (struct message_config){
        .bg = DEFAULT_BACKGROUND,
        .fg = COLOR_GREEN,
        .starter = &"> ",
        .new_lines_offset = 1,
    });
}

void unknown_command() {
    system_message("Unknown command!", (struct message_config){
        .bg = COLOR_RED,
        .fg = COLOR_WHITE,
        .starter = &"> ",
        .new_lines_offset = 1,
    });
}
