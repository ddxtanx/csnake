#include "screen.h"
#include "gtk_screen.h"
#include "../snake.h"
#include <gtk/gtk.h>
#include <stdio.h>

GtkApplication* app;

void activate_screen(GtkApplication* app, gpointer usr_data){
    GtkWidget* window;

    window = gtk_application_window_new(app);

    gtk_window_set_title((GtkWindow*)window, "App!");
    gtk_window_set_default_size((GtkWindow*)window, 400, 400);
    gtk_widget_show(window);
}

extern void draw_state(state* s){

}

extern void get_input(state* s){

}

extern void close_screen(){

}

extern void get_max_x_y(int* y, int* x){
    *y = 400;
    *x = 400;
}

extern void init_screen(){
    app = gtk_application_new("com.ddxtanx.snake", G_APPLICATION_FLAGS_NONE);

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