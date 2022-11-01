#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include "lib/snake.h"
#include "lib/screens/ncurses_screen.h"

pthread_t input_thread;
bool canceled = false;
state* state_p = NULL;
screen* screen_p = NULL;

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
    signal(SIGSTOP, (void*)handle_interrupt);
    signal(SIGTERM, (void*)handle_interrupt);

    screen scr;
    initialize_ncurses_screen(&scr);

    screen_p = &scr;

    scr.init_screen();

    int ROWS, COLS;
    scr.get_max_x_y(&ROWS, &COLS);

    if(argc == 3){
        char* rowsp;
        char* colsp;
        long arg_rows = strtol(argv[1], &rowsp, 10);
        int arg_cols = strtol(argv[2], &colsp, 10);

        if(rowsp != argv[1]){
            ROWS = arg_rows > ROWS ? ROWS : arg_rows;
        }
        
        if(colsp != argv[2]){
            COLS = arg_cols > COLS ? COLS : arg_cols;
        }
        
    }

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

        int diff = clock() - c;
        int remaining = (1.0/12 * 1000000) - diff;
        usleep(remaining);
    }

    pthread_cancel(thread);
    scr.close_screen();
    
    return 0;
}
