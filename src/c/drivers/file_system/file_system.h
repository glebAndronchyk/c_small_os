#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H
#include "../framebuffer/framebuffer.h"

#define MAX_FILE_CONTENT VGA_BUFFER_SIZE
#define MAX_FILES 10

struct file {
    char name[50];
    char content[MAX_FILE_CONTENT];
    int is_memory_used;
} ;

int create_file(char* name);
int list_files(char* name);
int write_file(char* name);
int read_file(char* name);
int execution_success_sequentially(char* name);
int delete_file(char* name);

#endif // FILE_SYSTEM_H
