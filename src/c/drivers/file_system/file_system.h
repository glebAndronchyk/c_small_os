#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H
#include "../framebuffer/framebuffer.h"

#define MAX_FILE_CONTENT VGA_WIDTH * VGA_HEIGHT
#define MAX_FILES 10

struct file {
    char name[50];
    char content[MAX_FILE_CONTENT];
    int is_memory_used;
} ;

int create_file(char* name);
int list_files(char* name);

#endif // FILE_SYSTEM_H
