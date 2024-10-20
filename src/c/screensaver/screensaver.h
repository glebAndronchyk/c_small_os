#ifndef SCREENSAVER_H
#define SCREENSAVER_H

struct star {
    int pos_row;
    int pos_col;
    char color;
    int flickering_speed;
    int current_tick;
    int can_move;
};

int start_screensaver();

#endif //SCREENSAVER_H
