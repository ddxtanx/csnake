#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "snake.h"

void init_state(state* state, int rows, int cols){
    time_t t;
    srand((unsigned) time(&t));
    state -> rows = rows;
    state -> cols = cols;

    int init_x = cols/2;
    int init_y = rows/2;

    snake_segment* head = malloc(sizeof(snake_segment));
    state -> head = head;
    head -> x = init_x;
    head -> y = init_y;
    head -> next_segment = NULL;

    snake_segment* tail = head;
    state -> tail = tail;

    enum Direction d = left;
    enum Direction copy_d = left;
    
    state -> cur_dir = d;
    state -> next_dir = copy_d;

    enum Tile** board = malloc(rows*sizeof(enum Tile*));
    for(int i = 0; i < rows; i++){
        board[i] = malloc(cols*sizeof(enum Tile));
    }

    state -> board = board;

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(i == init_y && j == init_x){
                board[i][j] = snake_head;
            } else{
                board[i][j] = empty;
            }
        }
    }

    do{
        state -> apple_x = rand() % cols;
        state -> apple_y = rand() % rows;
    } while(board[state -> apple_y][state -> apple_x] != empty);

    board[state -> apple_y][state -> apple_x] = apple;
    state -> lost = false;
    state -> ate_apple = false;
    state -> canceled = false;
}

void destroy_state(state* state){
    snake_segment*s = state -> head;
    while(s != NULL){
        snake_segment* temp = s -> next_segment;
        free(s);
        s = temp;
    }

    state -> head = NULL;
    state -> tail = NULL;

    for(int i = 0; i < state -> rows; i++){
        free(state -> board[i]);
        state -> board[i] = NULL;
    }

    free(state -> board);
    state -> board = NULL;
}

void advance_state(state* s){
    s -> cur_dir = s -> next_dir;

    snake_segment* cur = s -> head;
    int old_x = cur -> x;
    int old_y = cur -> y;

    switch(s -> cur_dir){
        case up:
            cur -> y -= 1;
            break;
        case down:
            cur -> y += 1;
            break;
        case left:
            cur -> x -= 1;
            break;
        case right:
            cur -> x += 1;
            break;
    }

    if(cur -> x >= s -> cols){
        cur -> x = 0;
    } else if(cur -> x < 0){
        cur -> x = s -> cols - 1;
    }

    if(cur -> y >= s -> rows){
        cur -> y = 0;
    } else if(cur -> y < 0){
        cur -> y = s -> rows - 1;
    }

    if(s -> board[cur -> y][cur -> x] == snake){
        s -> lost = true;
        return;
    }

    s -> board[cur -> y][cur -> x] = snake_head;
    cur = cur -> next_segment;

    while(cur != NULL){
        int temp_x = cur -> x;
        int temp_y = cur -> y;
        
        cur -> x = old_x;
        cur -> y = old_y;

        old_x = temp_x;
        old_y = temp_y;

        s -> board[cur -> y][cur -> x] = snake;
        cur = cur -> next_segment;
    }

    if(s -> ate_apple){
        snake_segment* new_seg = malloc(sizeof(snake_segment));
        new_seg -> x = old_x;
        new_seg -> y = old_y;
        new_seg -> next_segment = NULL;
        s -> board[old_y][old_x] = snake;

        s -> tail -> next_segment = new_seg;
        s -> tail = new_seg;
        s -> ate_apple = false;
    } else{
        s -> board[old_y][old_x] = empty;
    }

    if(s -> head -> x == s -> apple_x && s -> head -> y == s -> apple_y){
        s -> ate_apple = true;
        do{
            s -> apple_x = rand() % s -> cols;
            s -> apple_y = rand() % s -> rows;
        } while(s -> board[s -> apple_y][s -> apple_x] != empty);

        s -> board[s -> apple_y][s -> apple_x] = apple;
    }
}