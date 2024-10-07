#include "../framebuffer/framebuffer.h"
#include "../../utils/string_concat/string_concat.h"
#include "../../utils/string_copy/string_copy.h"
#include "../../utils/VGA_COLORS/VGA_COLORS.h"
#include "file_system.h"

struct file fs[MAX_FILES];

int create_file(char* name) {
    for (int  i = 0; i < MAX_FILES; i++) {
        if (!fs[i].is_memory_used) {

            fs[i] = (struct file) {
                .is_memory_used = 1,
                .content = '\0',
            };
            string_copy(fs[i].name, name);

            system_message("File created", COLOR_RED, COLOR_BLUE);
            return 1;
        }
    }

    return 0;
}

int list_files(char* noop) {
    int wrote = 0;

    system_message("root\0", COLOR_RED, COLOR_BLUE);

    for (int i = 0; i < MAX_FILES; i++) {
        if (fs[i].is_memory_used) {
            wrote++;

            char message[100] = "|-->";
            string_concat(message, fs[i].name);

            system_message(message, COLOR_RED, COLOR_BLUE);
        }
    }

    return wrote > 0;
}
