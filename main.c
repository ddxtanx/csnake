#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

enum Direction {up, down, left, right};
enum Tile {snake, snake_head, apple, empty};
pthread_t input_thread;
bool canceled = false;
enum Direction* cur_dir = NULL;

typedef struct __snake_segment {
    int x;
    int y;
    struct __snake_segment* next_segment;
} snake_segment;

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

void handle_input(enum Direction* d_ptr){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    while(true){
        int ch = getch();
        if(canceled){
            break;
        }
        switch(ch){
            case KEY_UP:
                if(cur_dir != NULL && *cur_dir != down){
                    *d_ptr = up;
                }
                break;
            case KEY_DOWN:
                if(cur_dir != NULL && *cur_dir != up){
                    *d_ptr = down;
                }
                break;
            case KEY_LEFT:
                if(cur_dir != NULL && *cur_dir != right){
                    *d_ptr = left;
                }
                break;
            case KEY_RIGHT:
                if(cur_dir != NULL && *cur_dir != left){
                    *d_ptr = right;
                }
                break;
            default:
                break;
        }
    }
}

int handle_interrupt(int signal){
    endwin();
    pthread_cancel(input_thread);
	canceled = true;
	return 0;
}

int main(int argc, char** argv){
    signal(SIGINT, (void*)handle_interrupt);
    signal(SIGSTOP, (void*)handle_interrupt);
    signal(SIGTERM, (void*)handle_interrupt);
    time_t t;
    srand((unsigned) time(&t));

    initscr();
    cbreak();
    keypad(stdscr, true);
    noecho();

    int ROWS, COLS;
    getmaxyx(stdscr, ROWS, COLS);

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
	move(0,0);
    int init_x = COLS/2;
    int init_y = ROWS/2;

    snake_segment* head = malloc(sizeof(snake_segment));
    head -> x = init_x;
    head -> y = init_y;
    head -> next_segment = NULL;

    snake_segment* tail = head;

    int apple_x;
    int apple_y;

    enum Direction d = left;
    enum Direction copy_d = left;
    cur_dir = &copy_d;

    enum Tile** board = malloc(ROWS*sizeof(enum Tile*));
    for(int i = 0; i < ROWS; i++){
        board[i] = malloc(COLS*sizeof(enum Tile));
    }

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            if(i == init_y && j == init_x){
                board[i][j] = snake_head;
            } else{
                board[i][j] = empty;
            }
        }
    }

    do{
        apple_x = rand() % COLS;
        apple_y = rand() % ROWS;
    } while(board[apple_y][apple_x] != empty);

    board[apple_y][apple_x] = apple;

    

    for(int x = 0; x < COLS; x++){
        for(int y = 0; y < ROWS; y++){
            mvaddch(y, x, '.');
        }
    }

    mvaddch(apple_y, apple_x, '@');
	refresh();
    pthread_t thread;
    pthread_create(&thread, NULL, (void*)handle_input, &d);
    input_thread = thread;

    bool ate_apple = false;

    clock_t c;
    while(true){
        c = clock();
        copy_d = d;
        int old_x = head -> x;
        int old_y = head -> y;

        switch(d){
            case up:
                head -> y -= 1;
                break;
            case down:
                head -> y += 1;
                break;
            case left:
                head -> x -= 1;
                break;
            case right:
                head -> x += 1;
                break;
        }

        if(head -> x >= COLS){
            head -> x = 0;
        } else if(head -> x < 0){
            head -> x = COLS - 1;
        }

        if(head -> y >= ROWS){
            head -> y = 0;
        } else if(head -> y < 0){
            head -> y = ROWS - 1;
        }

        if(board[head -> y][head -> x] == snake){
            break;
        }
        board[head -> y][head -> x] = snake_head;

        mvaddch(head -> y, head -> x, get_snake_head_given_dir(d));

        bool lost = false;
        for(snake_segment* s = head -> next_segment; s != NULL; s = s -> next_segment){
            int temp_x = s -> x;
            int temp_y = s -> y;
            
            s -> x = old_x;
            s -> y = old_y;

            old_x = temp_x;
            old_y = temp_y;

            board[s -> y][s -> x] = snake;
        }
        if(lost || canceled){
            break;
        }
        if(ate_apple){
            mvaddch(old_y, old_x, 'O');
            snake_segment* new_seg = malloc(sizeof(snake_segment));
            new_seg -> x = old_x;
            new_seg -> y = old_y;
            board[old_y][old_x] = snake;

            tail -> next_segment = new_seg;
            tail = new_seg;
            ate_apple = false;
        } else{
            board[old_y][old_x] = empty;
            mvaddch(old_y, old_x, '.');
        }

        if(head -> x == apple_x && head -> y == apple_y){
            ate_apple = true;

            do{
                apple_x = rand() % COLS;
                apple_y = rand() % ROWS;
            } while(board[apple_y][apple_x] != empty);

            board[apple_y][apple_x] = apple;
            mvaddch(apple_y, apple_x, '@');
        }

        move(head -> y, head -> x);
        refresh();

        int diff = clock() - c;
        int remaining = (1.0/12 * 1000000) - diff;
        usleep(remaining);
    }

    pthread_cancel(thread);
    endwin();
    
    return 0;
}
