#ifndef VGA_DISPLAY_H
#define VGA_DISPLAY_H

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BUFFER_SIZE (VGA_WIDTH * VGA_HEIGHT * 2)

struct message_config {
    char bg;
    char fg;
    char* starter;
    int new_lines_offset;
};

extern int current_row;
extern int current_col;
extern int cmd_length;

struct cursor_position {
    int row;
    int col;
};

struct buffer_save {
    char buffer_snapshot[VGA_BUFFER_SIZE];
    struct cursor_position cursor_position;
};

struct message_config;

void put_cursor(unsigned short pos);
int get_cursor();
int clear_framebuffer();
char get_char_at(unsigned short pos);
void clear_char(int char_offset);
void write_buffer_message(const char* message, unsigned char background, unsigned char foreground);
void scroll_framebuffer();
void new_line();
void new_line_with_starter(char* starter);
void controlled_new_line();
void system_message(const char *message,  struct message_config config);
char* prepare_message(const char val);
void save_buffer_content();
void restore_buffer();
void buffer_copy(char* dest, char* src);
void reset_positions();
void fill_sector(unsigned char background, int pos);
void fill_sector_with_char(unsigned foreground, int pos, char character);

#endif
