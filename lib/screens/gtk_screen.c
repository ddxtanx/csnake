#include "screen.h"
#include "gtk_screen.h"
#include "../snake.h"
#include <gtk/gtk.h>

GtkApplication* app;

void activate_screen(GtkApplication* app){

}

extern void draw_state(state* s){

}

extern void get_input(state* s){

}

extern void close_screen(){

}

extern void get_max_x_y(int* y, int* x){

}

extern void init_screen(){
    app = gtk_application_new("com.ddxtanx.snake", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(activate_screen), NULL);

    g_application_run(G_APPLICATION(app), 0, NULL);
}

void initialize_gtk_screen(screen *sc){
    sc -> close_screen = close_screen;
    sc -> init_screen = init_screen;
    sc -> draw_state = draw_state;
    sc -> get_input = get_input;
    sc -> get_max_x_y = get_max_x_y;
}