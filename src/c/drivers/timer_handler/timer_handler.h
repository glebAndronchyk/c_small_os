#ifndef TIMER_HANDLER_H
#define TIMER_HANDLER_H

#define TIMER_COMMANDS_COUNT 10

typedef void (*timer_command_handler)();

struct timer_command {
  char* name;
  int is_paused;
  timer_command_handler handler;
};

void add_timer_handler(timer_command_handler handler, char* name);
void pause_timer_handler(char* name);
void resume_timer_handler(char* name);
void remove_timer_handler(char* name);
void main_timer_handler();
int get_system_time();

#endif //TIMER_HANDLER_H
