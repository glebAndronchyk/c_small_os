#ifndef VGA_COLORS_H
#define VGA_COLORS_H

char get_color_word(unsigned char background, unsigned char foreground);

typedef enum {
    COLOR_BLACK = 0x0,
    COLOR_BLUE = 0x1,
    COLOR_GREEN = 0x2,
    COLOR_CYAN = 0x3,
    COLOR_RED = 0x4,
    COLOR_MAGENTA = 0x5,
    COLOR_BROWN = 0x6,
    COLOR_LIGHT_GRAY = 0x7,
    COLOR_DARK_GRAY = 0x8,
    COLOR_LIGHT_BLUE = 0x9,
    COLOR_LIGHT_GREEN = 0xA,
    COLOR_LIGHT_CYAN = 0xB,
    COLOR_LIGHT_RED = 0xC,
    COLOR_LIGHT_MAGENTA = 0xD,
    COLOR_YELLOW = 0xE,
    COLOR_WHITE = 0xF,


    DEFAULT_BACKGROUND = 0x0,
    DEFAULT_FOREGROUND = 0xA,
} VGA_Color;

#endif
