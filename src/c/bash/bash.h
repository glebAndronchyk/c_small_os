#ifndef BASH_H
#define BASH_H

#include "../drivers/framebuffer/framebuffer.h"
#include "../drivers/keyboard/keyboard.h"

#define COMMAND_BUFFER_SIZE VGA_WIDTH
#define MAX_COMMANDS 512

typedef int (*command_handler)(char* arg);

typedef struct {
    char name[COMMAND_BUFFER_SIZE];
    command_handler handler;
} commands;


extern commands command_table[MAX_COMMANDS];

void unknown_command();
void start_new_command();
char* read_data_in_row(int row);
void exec_command(const char* cmd_buffer);
void bash_key_handler(const struct keyboard_event event);
void show_title();
void init_bash();
void clear_commands();
void return_to_bash();
void give_control_to_app(void (*app_keyboard_handler)(struct keyboard_event event));

#endif // BASH_H
