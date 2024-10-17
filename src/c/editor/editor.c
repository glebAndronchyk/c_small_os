#include "../drivers/keyboard/keyboard.h"
#include "../drivers/file_system/file_system.h"
#include "../bash/bash.h"
#include "../utils/string_copy/string_copy.h"
#include "../utils/VGA_COLORS/VGA_COLORS.h"

char editor_buffer[VGA_BUFFER_SIZE];
int editor_buffer_index = 0;
struct file* used_file;

// TODO: fix naming overshadowing
void editor_backspace_pressed() {
    int char_offset = get_cursor() * 2;

    if (current_col > 0) {
        current_col--;
    } else if (current_row > 0) {
        current_row--;
        current_col = VGA_WIDTH;
    }

    clear_char(char_offset);
}

void write_char_in_editor_buffer(const char key) {
    editor_buffer[editor_buffer_index] = key;
    editor_buffer_index++;
}

void editor_go_new_line() {
    new_line();
    write_char_in_editor_buffer('\n');
}

void editor_char_pressed(char key) {
    write_char_in_editor_buffer(key);
    const char *msg = prepare_message(key);

    write_buffer_message(msg, 0xf, 0x1);
}

void exit_editor() {
    string_copy(used_file->content, editor_buffer);

    init_bash();
    restore_buffer();
    used_file = (struct file*) 0;
}

void editor_key_handler(const struct keyboard_event event) {
    if (event.type == EVENT_KEY_PRESSED) {
        switch (event.key) {
            case KEY_ENTER:
                return editor_go_new_line();
            case KEY_BACKSPACE:
                return editor_backspace_pressed();
            case KEY_ESC:
                return exit_editor();
            default:
                return editor_char_pressed(event.key_character);
        }
    }
}

void start_editor(struct file* file_data) {
    used_file = file_data;
    give_control_to_app(editor_key_handler);
    write_buffer_message(file_data->content, COLOR_WHITE, COLOR_BLACK);
}
