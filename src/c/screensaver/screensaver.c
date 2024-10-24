#include "screensaver.h"
#include "../drivers/keyboard/keyboard.h"
#include "../bash/bash.h"
#include "../drivers/timer/timer.h"
#include "../utils/random/random.h"
#include "../utils/VGA_COLORS/VGA_COLORS.h"

#define  MAX_STARS 150

struct star stars[MAX_STARS];
VGA_Color star_colors[] = {COLOR_BLUE, COLOR_RED, COLOR_GREEN, COLOR_CYAN, COLOR_BROWN, COLOR_MAGENTA};

void exit_screensaver() {
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i] = (struct star){0};
    }
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
        star_impl.color = star_colors[get_random_in_range(0, 5)];
        star_impl.flickering_speed = get_random_in_range(100, 130);
        star_impl.current_tick = get_random_in_range(20, 100);
        star_impl.can_move = get_random_in_range(0, 20);

        stars[i] = star_impl;
    }
}

void draw_star(struct star* star) {
    fill_sector_with_char(star->color, (star->pos_row * VGA_WIDTH + star->pos_col) * 2, '*');
}

void move_star(struct star* star, int offset) {
    VGA_Color prev_color = star->color;

    star->color = COLOR_BLACK;
    draw_star(star);

    star->pos_col += offset;
    if (star->pos_col >= VGA_WIDTH || star->pos_col < 0) {
        star->pos_col -= 2 * offset;
    }
    star->color = prev_color;
}

void draw_stars() {
    for (int i = 0; i < MAX_STARS; i++) {
        struct star* star = &stars[i];
        draw_star(star);
    }
}

void process_star_tick(struct star* star, int offset) {
    star->color += rgb_offset * offset;
    star->can_move > 10 ? move_star(star, offset) : 1;
    draw_star(star);
    star->current_tick = offset > 0 ? star->current_tick + 1 : get_random_in_range(20, 100);
}

void screensaver_timer_handler() {
    for (int i = 0; i < MAX_STARS; i++) {
        struct star* star = &stars[i];

        if (star->current_tick == star->flickering_speed) {
            process_star_tick(star, 1);
        } else if (star->current_tick > star->flickering_speed + 5) {
            process_star_tick(star, -1);
        } else {
            star->current_tick++;
        }
    }
}

int start_screensaver() {
    give_control_to_app(screensaver_key_handler, 1);
    init_stars();
    timer_set_handler(screensaver_timer_handler);
    draw_stars();

    return 1;
}
