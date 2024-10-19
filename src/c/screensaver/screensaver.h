#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#define MAX_STARS 300

struct star {
    int pos_row;
    int pos_col;
    char color;
    int flickering_speed;
};

extern struct star stars[MAX_STARS];

int start_screensaver();

#endif //SCREENSAVER_H
