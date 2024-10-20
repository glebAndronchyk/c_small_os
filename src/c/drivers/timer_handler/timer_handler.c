#include "timer_handler.h"

#include "../../bash/messages/messages.h"
#include "../../utils/cmp_string/cmp_string.h"
#include "../../utils/string_copy/string_copy.h"

int debug_tick = 0;
int timer_command_idx = 0;
int system_time = 0;
struct timer_command* timer_command_stack[TIMER_COMMANDS_COUNT + 1];

int get_system_time() {
    return system_time;
}

void pause_timer_handler(char* name) {
    for (int i = 0; i < timer_command_idx; i++) {
        struct timer_command* cmd = timer_command_stack[i];

        if (cmp_string(cmd->name, name)) {
            cmd->is_paused = 1;
            break;
        }
    }
}

void resume_timer_handler(char* name) {
    for (int i = 0; i < timer_command_idx; i++) {
        struct timer_command* cmd = timer_command_stack[i];

        if (cmp_string(cmd->name, name)) {
            cmd->is_paused = 0;
            break;
        }
    }
}

void remove_timer_handler(char* name) {
    for (int i = 0; i < timer_command_idx; i++) {
        if (cmp_string(timer_command_stack[i]->name, name)) {
            for (int j = i; j < timer_command_idx - 1; j++) {
                timer_command_stack[j] = timer_command_stack[j + 1];
            }

            timer_command_idx--;
            break;
        }
    }
}

void add_timer_handler(timer_command_handler handler, char* name) {
    if (timer_command_idx < TIMER_COMMANDS_COUNT) {
        struct timer_command cmd = {
            .handler = handler,
            .is_paused = 0,
        };
        string_copy(cmd.name, name);

        timer_command_stack[timer_command_idx++] = &cmd;
    }
}

void main_timer_handler() {
    if (debug_tick > 30) {
        execution_success(timer_command_stack[1]->name);
        debug_tick = 0;
    } else {
        debug_tick++;
    }

    system_time++;
    for (int i = 1; i < timer_command_idx; i++) {
        struct timer_command* cmd = timer_command_stack[i];

        if (!cmd->is_paused) {
            cmd->handler();
        }
    }
}
