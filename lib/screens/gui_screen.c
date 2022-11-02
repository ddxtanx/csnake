#include "screen.h"
#include "gui_screen.h"
#include "../snake.h"
#include <stdio.h>
#include <pthread.h>
#include <raylib.h>

extern void draw_state(state* s){
    if(WindowShouldClose()){
        printf("Hi!\n");
        s -> canceled = true;
    }
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
}

extern void get_input(state* s){

}

extern void close_screen(){
    CloseWindow();
}

extern void get_max_x_y(int* y, int* x){
    int scr_height = GetScreenHeight();
    int scr_width = GetScreenWidth();
    *y = scr_height;
    *x = scr_width;
}

extern void init_screen(int width, int height){
    InitWindow(width, height, "Snake!");
}

void initialize_gui_screen(screen *sc){
    sc -> close_screen = close_screen;
    sc -> init_screen = init_screen;
    sc -> draw_state = draw_state;
    sc -> get_input = get_input;
    sc -> get_max_x_y = get_max_x_y;
}