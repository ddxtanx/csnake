#include "screen.h"
#include "gui_screen.h"
#include "../snake.h"
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <raylib.h>

int w = 0;
int h = 0;
extern void draw_state_gui(state* s){
    int box_height = h/s -> rows;
    int box_width = w / s -> cols;
    if(WindowShouldClose()){
        printf("Hi!\n");
        s -> canceled = true;
    }
    BeginDrawing();

    for(int i = 0; i < s -> rows; i++){
        for(int j = 0; j < s -> cols; j++){
            int x = j * box_width;
            int y = i * box_height;
            Color c;
            enum Tile t = s -> board[i][j];
            switch(t){
                case empty:
                    c = GRAY;
                    break;
                case snake_head:
                    c = PINK;
                    break;
                case snake:
                    c = PURPLE;
                    break;
                case apple:
                    c = GREEN;
                    break;
            }

            DrawRectangle(x, y, box_width, box_height, c);
        }
    }

    EndDrawing();
}

extern void get_input_gui(state* s){
    while(true){
        if(s -> canceled || s -> lost){
            break;
        }
        int c = GetKeyPressed();
        switch(c){
            case KEY_UP:
                if(s -> cur_dir != down){
                    s -> next_dir = up;
                }
                break;
            case KEY_DOWN:
                if(s -> cur_dir != up){
                    s -> next_dir = down;
                }
                break;
            case KEY_LEFT:
                if(s -> cur_dir != right){
                    s -> next_dir = left;
                }
                break;
            case KEY_RIGHT:
                if(s -> cur_dir != left){
                    s -> next_dir = right;
                }
                break;
            default:
                break;
        }
    }
}

extern void close_screen_gui(){
    CloseWindow();
}

extern void get_max_x_y_gui(int* y, int* x){
    InitWindow(0, 0, "Snake!");
    int scr_height = GetScreenHeight();
    int scr_width = GetScreenWidth();
    *y = scr_height;
    *x = scr_width;
}

extern void init_screen_gui(int width, int height){
    w = width;
    h = height;
    printf("Width: %d, Height: %d\n", width, height);
    SetWindowSize(width, height);
}

void initialize_gui_screen(screen *sc){
    sc -> close_screen = close_screen_gui;
    sc -> init_screen = init_screen_gui;
    sc -> draw_state = draw_state_gui;
    sc -> get_input = get_input_gui;
    sc -> get_max_x_y = get_max_x_y_gui;
}