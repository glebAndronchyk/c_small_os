#include "framebuffer.h"

#include "../../bash/messages/messages.h"
#include "../../kernel/kernel.h"
#include "../../utils/cmp_string/cmp_string.h"
#include "../../utils/string_length/string_length.h"
#include "../../utils/VGA_COLORS/VGA_COLORS.h"

int current_row = 0;
int current_col = 0;
int cmd_length = 0;

struct buffer_save latest_save;

void put_cursor(unsigned short pos) {
    out(0x3D4, 14);
    out(0x3D5, ((pos >> 8) & 0x00FF));
    out(0x3D4, 15);
    out(0x3D5, pos & 0x00FF);
}

int get_cursor() {
    return (current_row * VGA_WIDTH + current_col);
}

void scroll_if_overflows() {
    if (current_row >= VGA_HEIGHT) {
        scroll_framebuffer();
    }
}

int clear_framebuffer() {
    volatile char *framebuffer = (volatile char*) VGA_ADDRESS;

    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        *framebuffer = ' ';
        *(framebuffer + 1) = get_color_word(COLOR_BLACK, DEFAULT_FOREGROUND);
        framebuffer += 2;
    }

    reset_positions();
    put_cursor(get_cursor());

    return 1;
}

void reset_positions() {
    current_row = 0;
    current_col = 0;
}

char get_char_at(unsigned short pos) {
    volatile char *framebuffer = (volatile char*) VGA_ADDRESS;

    return framebuffer[pos];
}

void clear_char(int char_offset) {
    volatile char *framebuffer = (volatile char*) VGA_ADDRESS;

    framebuffer[char_offset] = ' ';
    framebuffer[char_offset + 1] = get_color_word(COLOR_BLACK, DEFAULT_FOREGROUND);
    put_cursor(get_cursor());
}

void write_buffer_message(const char* message, unsigned char background, unsigned char foreground) {
    volatile char *framebuffer = (volatile char*) VGA_ADDRESS;

    while (*message != '\0') {
        if (*message == '\n') {
            message++;
            new_line();
            continue;
        };

        int char_offset = get_cursor() * 2;
        framebuffer[char_offset++] = *message;
        framebuffer[char_offset] = get_color_word(background, foreground);

        message++;
        current_col++;
        controlled_new_line();
    }

    put_cursor(get_cursor());
}

void scroll_framebuffer() {
    volatile char *framebuffer = (volatile char*) VGA_ADDRESS;

    for (int row = 0; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            const int source_offset = (row * VGA_WIDTH + col) * 2;
            const int destination_offset = ((row - 1) * VGA_WIDTH + col) * 2;

            framebuffer[destination_offset] = framebuffer[source_offset];
            framebuffer[destination_offset + 1] = framebuffer[source_offset + 1];
        }
    }

    for (int col = 0; col < VGA_WIDTH; col ++) {
        int last_row_offset = ((VGA_HEIGHT - 1) * VGA_WIDTH + col) * 2;

        framebuffer[last_row_offset] = ' ';
        framebuffer[last_row_offset + 1] = get_color_word(DEFAULT_BACKGROUND, DEFAULT_FOREGROUND);
    }

    current_row = VGA_HEIGHT - 1;
    current_col = 0;
    put_cursor(get_cursor());
}

void controlled_new_line() {
    if (current_col > VGA_WIDTH) {
        new_line();
    }
}

void new_line_with_starter(char* starter) {
    new_line();
    write_buffer_message(starter, DEFAULT_BACKGROUND, DEFAULT_FOREGROUND);
}

void new_line() {
    current_row++;
    current_col = 0;
    scroll_if_overflows();
    put_cursor(get_cursor());
}

void system_message(const char *message, struct message_config config) {
    write_buffer_message(message, config.bg, config.fg);
    for (int i = 0; i < config.new_lines_offset; i++) {
        new_line();
    }
    new_line_with_starter(config.starter);
    put_cursor(get_cursor());
}

char* prepare_message(const char val) {
    static char msg[2];
    msg[0] = val;
    msg[1] = '\0';
    return msg;
}

void buffer_copy(char* dest, char* src) {
    for (int i = 0; i < VGA_BUFFER_SIZE; i ++) {
        dest[i] = src[i];
    }
}

void save_buffer_content() {
    char *framebuffer = (char*) VGA_ADDRESS;
    latest_save = (struct buffer_save) {};
    buffer_copy(latest_save.buffer_snapshot, framebuffer);
    latest_save.cursor_position = (struct cursor_position) {
        .col = current_col,
        .row = current_row,
    };
}

void restore_position_from_latest_save() {
    current_col = latest_save.cursor_position.col;
    current_row = latest_save.cursor_position.row;

    latest_save.cursor_position = (struct cursor_position) {
        .col = 0,
        .row = 0,
    };

    write_buffer_message("> ", DEFAULT_BACKGROUND, DEFAULT_FOREGROUND);
    put_cursor(get_cursor());
}

void restore_buffer() {
    char *framebuffer = (char*) VGA_ADDRESS;
    for (int i = 0; i < VGA_BUFFER_SIZE; i++) {
        framebuffer[i] = latest_save.buffer_snapshot[i];
    }
    restore_position_from_latest_save();
}
