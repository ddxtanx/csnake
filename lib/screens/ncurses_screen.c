#include "screen.h"
#include "ncurses_screen.h"
#include "../snake.h"
#include <ncurses.h>
#include <pthread.h>

char get_snake_head_given_dir(enum Direction d){
    switch(d){
        case up:
            return '^';
            break;
        case down:
            return 'V';
            break;
        case left:
            return '<';
            break;
        case right:
            return '>';
            break;
    }
}


extern void draw_state(state* s){
    enum Tile** board = s -> board;
    //clear();
    for(int i = 0; i < s -> rows; i++){
        for(int j = 0; j < s -> cols; j++){
            enum Tile t = board[i][j];

            switch(t){
                case empty:
                    mvaddch(i, j, '.');
                    break;
                case snake_head:
                    mvaddch(i, j, get_snake_head_given_dir(s -> cur_dir));
                    break;
                case snake:
                    mvaddch(i, j, 'O');
                    break;
                case apple:
                    mvaddch(i, j, '@');
                    break;
            }
            // usleep(.1 * 1000000);
            // refresh();
            if(s -> canceled){
                break;
            }
        }
        if(s -> canceled){
            break;
        }
    }

    refresh();
}
extern void handle_input(state* s){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    while(true){
        int ch = getch();
        if(s -> canceled){
            break;
        }
        switch(ch){
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

extern void close_screen(){
    endwin();
}

extern void init_screen(int width, int height){
    initscr();
    cbreak();
    keypad(stdscr, true);
    noecho();

    move(0,0);
}

extern void get_max_x_y(int* y, int* x){
    int r, c;
    getmaxyx(stdscr, r, c);

    *y = r;
    *x = c;
}

void initialize_ncurses_screen(screen *sc){

    sc -> init_screen = init_screen;
    sc -> get_input = handle_input;
    sc -> draw_state = draw_state;
    sc -> close_screen = close_screen;
    sc -> get_max_x_y = get_max_x_y;
}
