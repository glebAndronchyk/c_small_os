#include "../framebuffer/framebuffer.h"
#include "../../editor/editor.h"
#include "../../utils/string_concat/string_concat.h"
#include "../../utils/string_copy/string_copy.h"
#include "../../utils/string_length/string_length.h"
#include "../../utils/VGA_COLORS/VGA_COLORS.h"
#include "file_system.h"

#include <stddef.h>

#include "../../bash/messages/messages.h"
#include "../../utils/cmp_string/cmp_string.h"

struct file fs[MAX_FILES];

struct file* get_file(char* file_name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (cmp_string(fs[i].name, file_name)) {
            return &fs[i];
        }
    }

    return NULL;
}

int delete_file(char* name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (cmp_string(fs[i].name, name)) {
            if (fs[i].is_memory_used) {
                fs[i].is_memory_used = 0;
                string_copy(fs[i].name, "");
                string_copy(fs[i].content, "");
                execution_success("File deleted");
                return 1;
            }
        }
    }
    execution_error("File not found");
    return 0;
}

int create_file(char* name) {
    if (string_length(name) != 0) {
        for (int  i = 0; i < MAX_FILES; i++) {
            // if (cmp_string(name, fs[i].name)) {
            //     break;
            // }

            if (!fs[i].is_memory_used) {
                fs[i] = (struct file) {
                    .is_memory_used = 1,
                };
                string_copy(fs[i].content, "");
                string_copy(fs[i].name, name);

                execution_success("File created");
                return 1;
            }
        }
    }

    return 0;
}

int list_files(char* noop) {
    execution_success_sequentially("root\0");

    for (int i = 0; i < MAX_FILES; i++) {
        if (fs[i].is_memory_used) {
            char message[100] = "|-->";
            string_concat(message, fs[i].name);

            execution_success_sequentially(message);
        }
    }

    new_line_with_starter("> ");

    return 1;
}

int write_file(char* file_name) {
    struct file* file_to_write = get_file(file_name);
    if (file_to_write->is_memory_used) {
        start_editor(file_to_write);
        return 1;
    }

    return 0;
}

int read_file(char* file_name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (cmp_string(fs[i].name, file_name)) {
            if (string_length(fs[i].content) != 0) {
                execution_success(fs[i].content);
            } else {
                execution_error("File is empty");
            }

            return 1;
        }
    }
    return 0;
}

