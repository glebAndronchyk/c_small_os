#include "../drivers/file_system/file_system.h"
#include "../drivers/framebuffer/framebuffer.h"
#include "../utils/cmp_string/cmp_string.h"
#include "../drivers/keyboard/keyboard.h"
#include "bash.h"
#include "../utils/string_length/string_length.h"
#include "./messages/messages.h";

commands command_table[MAX_COMMANDS] = {
    { "clear", clear_commands},
    { "create", create_file},
    { "list", list_files},
    { "read", read_file},
    { "restore-vga", restore_buffer},
    { "write", write_file}
};

void clear_commands() {
    clear_framebuffer();
    app_name();
}

void start_new_command() {
    cmd_length = 0;
}

char* read_data_in_row(int row) {
    const int row_pos = row * VGA_WIDTH * 2;
    static char message[COMMAND_BUFFER_SIZE];
    int buffer_idx = 0;

    for (int col = 0; col < VGA_WIDTH; col++) {
        const char ch = get_char_at(row_pos + col * 2);
        const char chNext = get_char_at(row_pos + (col + 1) * 2);

        if (ch == '>' && chNext == ' ') {
            col++;
            continue;
        }

        if (ch == ' ' && chNext == ' ') {
            message[buffer_idx] = '\0';
            break;
        }

        message[buffer_idx++] = ch;
        if (buffer_idx >= COMMAND_BUFFER_SIZE - 1) {
            message[buffer_idx] = '\0';
            break;
        }
    }

    return message;
}

void process_command_result(int result) {
    if (result) {
        start_new_command();
    } else {
        execution_error("Failed to execute command");
    }
}

typedef struct {
    char command[COMMAND_BUFFER_SIZE];
    char argument[COMMAND_BUFFER_SIZE];
} CommandArg;

CommandArg split_command_and_arg(const char* cmd_buffer) {
    CommandArg result;
    int i = 0;
    int j = 0;

    while (cmd_buffer[i] != ' ' && cmd_buffer[i] != '\0' && i < COMMAND_BUFFER_SIZE) {
        result.command[i] = cmd_buffer[i];
        i++;
    }
    result.command[i] = '\0';

    if (cmd_buffer[i] == ' ') {
        i++;

        while (cmd_buffer[i] != '\0' && j < COMMAND_BUFFER_SIZE) {
            result.argument[j++] = cmd_buffer[i++];
        }
        result.argument[j] = '\0';
    } else {
        result.argument[0] = '\0';
    }

    return result;
}

void exec_command(const char* cmd_buffer) {
    new_line();

    if (string_length(cmd_buffer) != 0) {
        CommandArg command_meta = split_command_and_arg(cmd_buffer);
        for (int k = 0; k < MAX_COMMANDS; k++) {
            if (cmp_string(command_table[k].name, command_meta.command)) {
                const int result = command_table[k].handler(command_meta.argument);
                return process_command_result(result);
            }
        }

        unknown_command();
        start_new_command();
    }
}

void backspace_pressed() {
    int char_offset = get_cursor() * 2;

    if (current_col > 0) {
        current_col--;
    }

    clear_char(char_offset);
}

void enter_pressed() {
    char *cmd_buffer = read_data_in_row(current_row);
    exec_command(cmd_buffer);
}

void char_pressed(char key) {
    if (cmd_length == VGA_WIDTH) {
        cmd_length = 0;
        execution_error("Message overflows buffer");
        current_row++;
        put_cursor(get_cursor());
        return;
    }

    cmd_length++;
    const char *msg = prepare_message(key);

    char_press(msg);
}

void bash_key_handler(const struct keyboard_event event) {
    if (event.key_character && event.type == EVENT_KEY_PRESSED) {
        switch (event.key) {
            case KEY_ENTER:
                return enter_pressed();
            case KEY_BACKSPACE:
                return backspace_pressed();
            default:
                return char_pressed(event.key_character);
        }
    }
}

void give_control_to_app(void (*app_keyboard_handler)(struct keyboard_event event)) {
    save_buffer_content();
    clear_framebuffer();
    keyboard_set_handler(app_keyboard_handler);
}

void init_bash() {
    keyboard_set_handler(bash_key_handler);
    clear_framebuffer();
    app_name();
}
