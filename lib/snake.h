#pragma once
#include <stdlib.h>
#include <stdbool.h>

enum Direction {up, down, left, right};
enum Tile {empty, snake_head, apple, snake};

typedef struct __snake_segment {
    int x;
    int y;
    struct __snake_segment* next_segment;
} snake_segment;

typedef struct __state {
    enum Tile** board;

    enum Direction cur_dir;
    enum Direction next_dir;

    snake_segment* head;
    snake_segment* tail;

    int rows;
    int cols;

    bool lost;
    bool ate_apple;
    bool canceled;

    int apple_x;
    int apple_y;
} state;

void init_state(state* state, int rows, int cols);
void destroy_state(state* state);
void advance_state(state* state);