#include "screensaver.h"
#include "../drivers/keyboard/keyboard.h"
#include "../bash/bash.h"
#include "../bash/messages/messages.h"
#include "../drivers/timer_handler/timer_handler.h"
#include "../utils/random/random.h"
#include "../utils/VGA_COLORS/VGA_COLORS.h"

#define  MAX_STARS 150

struct star stars[MAX_STARS];
VGA_Color star_colors[] = {COLOR_BLUE, COLOR_RED, COLOR_GREEN, COLOR_CYAN, COLOR_BROWN, COLOR_MAGENTA};

void exit_screensaver() {
    // TODO: not working
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i] = (struct star){0};
    }
    return_to_bash();
    pause_timer_handler("screensaver");
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
        star_impl.color = star_colors[get_random_in_range(0, 5)];
        star_impl.flickering_speed = get_random_in_range(100, 130);
        star_impl.current_tick = get_random_in_range(20, 100);
        // star_impl.can_move = get_random_in_range(0, 1);

        stars[i] = star_impl;
    }
}

void draw_star(struct star* star) {
    fill_sector_with_char(star->color, (star->pos_row * VGA_WIDTH + star->pos_col) * 2, '*');
}

void draw_stars() {
    for (int i = 0; i < MAX_STARS; i++) {
        struct star* star = &stars[i];
        draw_star(star);
    }
}

// executes when paused
void screensaver_timer_handler() {
    // if (debug_tick > 10) {
    //     execution_success("a");
    //     debug_tick = 0;
    // } else {
    //     debug_tick++;
    // }

    for (int i = 0; i < MAX_STARS; i++) {
        struct star* star = &stars[i];

        if (star->current_tick == star->flickering_speed) {
            star->color += rgb_offset;
            // star->pos_col = star->can_move ? star->pos_row + 1 : 0;
            draw_star(star);
            star->current_tick++;
        } else if (star->current_tick > star->flickering_speed + 5) {
            star->color -= rgb_offset;
            // star->pos_col = star->can_move ? star->pos_row - 1 : 0;
            draw_star(star);
            star->current_tick = get_random_in_range(20, 100);
        } else {
            star->current_tick++;
        }
    }
}

int start_screensaver() {
    pause_timer_handler("sleep");
    give_control_to_app(screensaver_key_handler);
    init_stars();
    add_timer_handler(screensaver_timer_handler, "screensaver");
    draw_stars();

    return 1;
}
