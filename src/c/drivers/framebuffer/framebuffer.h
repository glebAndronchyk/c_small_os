#ifndef VGA_DISPLAY_H
#define VGA_DISPLAY_H

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

extern int current_row;
extern int current_col;
extern int cmd_length;

void put_cursor(unsigned short pos);
int get_cursor();
int clear_framebuffer();
char get_char_at(unsigned short pos);
void clear_char(int char_offset);
void write_buffer_message(const char* message, unsigned char background, unsigned char foreground);
void scroll_framebuffer();
void new_line();
void system_message(const char *message, unsigned char background, unsigned char foreground);
char* prepare_message(const char val);

#endif
