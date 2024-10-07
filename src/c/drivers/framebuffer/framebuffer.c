#include "framebuffer.h"
#include "../../kernel/kernel.h"

int current_row = 0;
int current_col = 0;
int cmd_length = 0;

void put_cursor(unsigned short pos) {
    out(0x3D4, 14);
    out(0x3D5, ((pos >> 8) & 0x00FF));
    out(0x3D4, 15);
    out(0x3D5, pos & 0x00FF);
}

int get_cursor() {
    return (current_row * VGA_WIDTH + current_col);
}

int clear_framebuffer() {
    volatile char *framebuffer = (volatile char*) VGA_ADDRESS;

    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        *framebuffer = ' ';
        *(framebuffer + 1) = (0x0 << 4) | 0xF;
        framebuffer += 2;
    }

    current_row = 0;
    current_col = 0;
    put_cursor(get_cursor());

    return 1;
}

char get_char_at(unsigned short pos) {
    volatile char *framebuffer = (volatile char*) VGA_ADDRESS;

    return framebuffer[pos];
}

void clear_char(int char_offset) {
    volatile char *framebuffer = (volatile char*) VGA_ADDRESS;

    framebuffer[char_offset] = ' ';
    framebuffer[char_offset + 1] = (0x0 << 4) | 0xF;
    put_cursor(get_cursor());
}

void write_buffer_message(const char* message, unsigned char background, unsigned char foreground) {
    int char_offset = get_cursor() * 2;
    volatile char *framebuffer = (volatile char*) VGA_ADDRESS;

    while (*message != '\0') {
        framebuffer[char_offset++] = *message;
        framebuffer[char_offset++] = background << 4 | foreground & 0x0F;

        message++;
        current_col++;
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
        framebuffer[last_row_offset + 1] = (0x0 << 4) | 0xF;;
    }

    current_row = VGA_HEIGHT - 1;
    current_col = 0;
    put_cursor(get_cursor());
}

void new_line() {
    current_row++;
    current_col = 0;

    if (current_row >= VGA_HEIGHT) {
        scroll_framebuffer();
    }
}

void system_message(const char *message, unsigned char background, unsigned char foreground) {
    write_buffer_message(message, background, foreground);
    new_line();
    put_cursor(get_cursor());
}

char* prepare_message(const char val) {
    static char msg[2];
    msg[0] = val;
    msg[1] = '\0';
    return msg;
}

