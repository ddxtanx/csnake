#pragma once
#include "../snake.h"

typedef struct __screen{
    void (*init_screen)(int width, int height);
    void (*draw_state)(state* s);
    void (*get_input)(state* s);
    void (*close_screen)();

    void (*get_max_x_y)(int* y, int* x);
} screen;