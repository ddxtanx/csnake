#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include "lib/snake.h"
#include "lib/screens/ncurses_screen.h"
#include "lib/screens/gui_screen.h"
#include <pthread.h>

pthread_t input_thread;
bool canceled = false;
state* state_p = NULL;
screen* screen_p = NULL;

int default_width = 500;
int default_height = 500;

int default_rows = 20;
int default_cols = 20;

int handle_interrupt(int signal){
    if(screen_p != NULL){
        screen_p -> close_screen();
    }
    pthread_cancel(input_thread);
    if(state_p != NULL){
        state_p -> canceled = true;
    }
    return 0;
}

int main(int argc, char** argv){
    signal(SIGINT, (void*)handle_interrupt);
    signal(SIGTERM, (void*)handle_interrupt);

    screen scr;
    bool gui_mode = true;

    //initialize_ncurses_screen(&scr);
    if(gui_mode){
        initialize_gui_screen(&scr);
    } else{
        initialize_ncurses_screen(&scr);
    }
    
    screen_p = &scr;

    

    int WIDTH, HEIGHT;
    scr.get_max_x_y(&WIDTH, &HEIGHT);

    int ROWS = default_rows;
    int COLS = default_cols;

    if(argc >= 3){
        char* rowsp;
        char* colsp;
        long arg_width = strtol(argv[1], &rowsp, 10);
        int arg_height = strtol(argv[2], &colsp, 10);

        if(rowsp != argv[1]){
            HEIGHT = arg_height > HEIGHT ? HEIGHT : arg_height;
        }
        
        if(colsp != argv[2]){
            WIDTH = arg_width > WIDTH ? WIDTH : arg_width;
        }
    } else if(gui_mode){
        HEIGHT = default_height > HEIGHT ? HEIGHT : default_height;
        WIDTH = default_width > WIDTH ? WIDTH : default_width;
    } else{
        HEIGHT = default_rows > HEIGHT ? HEIGHT : default_rows;
        WIDTH = default_cols > WIDTH ? WIDTH : default_cols;
    }

    if(argc == 5){
        char* rowsp;
        char* colsp;
        long arg_rows = strtol(argv[3], &rowsp, 10);
        int arg_cols = strtol(argv[4], &colsp, 10);

        if(rowsp != argv[3]){
            ROWS = arg_rows;
        }
        if(colsp != argv[4]){
            COLS = arg_cols;
        }
    } 

    scr.init_screen(WIDTH, HEIGHT);
    printf("Init'd screen\n");


    state s;
    init_state(&s, ROWS, COLS);
    state_p = &s;
    
    scr.draw_state(&s);

    pthread_t thread;
    pthread_create(&thread, NULL, (void*)scr.get_input, &s);
    input_thread = thread;

    bool ate_apple = false;

    clock_t c;
    while(true){
        c = clock();
        if(s.canceled || s.lost){
            break;
        }

        advance_state(&s);
        scr.draw_state(&s);
        //printf("Drawing\n");

        int diff = clock() - c;
        int remaining = (1.0/12 * 1000000) - diff;
        usleep(remaining);
    }
    scr.close_screen();
    destroy_state(&s);
    pthread_cancel(thread);
    pthread_join(thread, NULL);
    
    
    return 0;
}
