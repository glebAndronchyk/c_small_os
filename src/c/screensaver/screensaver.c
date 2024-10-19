#include "screensaver.h"
#include "../drivers/keyboard/keyboard.h"
#include "../bash/bash.h"
#include "../utils/random/random.h"
#include "../utils/VGA_COLORS/VGA_COLORS.h"

struct star stars[MAX_STARS];

void exit_screensaver() {
    return_to_bash();
}

void screensaver_key_handler(const struct keyboard_event event) {
    if (event.type == EVENT_KEY_PRESSED) {
        exit_screensaver();
    }
}

void init_stars() {
    for (int i = 0; i < MAX_STARS; i++) {
        struct star star_impl = {};

        star_impl.pos_row =  get_random_in_range(0, VGA_HEIGHT - 1);
        star_impl.pos_col =  get_random_in_range(0, VGA_WIDTH);
        star_impl.color = COLOR_RED;
        star_impl.flickering_speed = 1;

        stars[i] = star_impl;
    }
}

void draw_stars() {
    for (int i = 0; i < MAX_STARS; i++) {
        struct star star = stars[i];
        fill_sector_with_char(star.color, (star.pos_row * VGA_WIDTH + star.pos_col) * 2, '*');
    }
}

int start_screensaver() {
    give_control_to_app(screensaver_key_handler);
    init_stars();
    draw_stars();

    return 1;
}
