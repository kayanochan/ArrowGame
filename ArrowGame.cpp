#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

#define BLACK	 al_map_rgb(0, 0, 0)
#define WHITE	 al_map_rgb(255, 255, 255)
#define BLUE     al_map_rgb(197, 214, 237)

//Initialize constants
const float FPS = 200;
// Screen dimensions
const int SW = 1600;
const int SH = 900;

int main(int argc, char *argv[]) {

    srand(time(0));

    ALLEGRO_DISPLAY *display = nullptr;
    ALLEGRO_DISPLAY_MODE   disp_data;
    // Add event queue object
	ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
	// Add timer object
	ALLEGRO_TIMER *timer = nullptr;

	// Initialize Allegro
	al_init();
	// Initialize display
    al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    display = al_create_display(disp_data.width, disp_data.height);
	if (!display) {
    	al_show_native_message_box(display, "Error", "Error",
                                "Failed to initialize display!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
       	return -1;
	}
	al_set_window_title(display, "Arrow Game");
	// Initialize keyboard routines
	if (!al_install_keyboard()) {
	    al_show_native_message_box(display, "Error", "Error",
                                "Failed to initialize the keyboard!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
     	return -1;
   	}
    // Initialize mouse routines
    if(!al_install_mouse()) {
	    al_show_native_message_box(display, "Error", "Error",
                                "Failed to initialize the mouse!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
     	return -1;
    }
    // Initialize the image processor
 	if (!al_init_image_addon()) {
    	al_show_native_message_box(display, "Error", "Error",
                                "Failed to initialize the image add-on!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    	return -1;
	}
	// Set up the timer
	timer = al_create_timer(1.0 / FPS);
   	if (!timer) {
   		al_show_native_message_box(display, "Error", "Error",
                                "Failed to create the timer!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
	// Set up the event queue
	event_queue = al_create_event_queue();
	if (!event_queue) {
		al_show_native_message_box(display, "Error", "Error",
                             "Failed to create the event queue!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
      	return -1;
	}
	if (!al_init_primitives_addon()) {
    	al_show_native_message_box(display, "Error", "Error",
                                "Failed to initialize primatives add-on!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    	return -1;
	}
	al_init_font_addon(); // initialize the font add on
    al_init_ttf_addon();// initialize the ttf (True Type Font) add on
    ALLEGRO_FONT *font = al_load_ttf_font("comic.ttf", 50, 0);
    if (!font){
        al_show_native_message_box(display, "Error", "Error",
                                   "Could not load comic.ttf!",
                                    nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    // Images
    ALLEGRO_BITMAP *iArrow[4];
    iArrow[0] = al_load_bitmap("right.bmp");
    iArrow[1] = al_load_bitmap("up.bmp");
    iArrow[2] = al_load_bitmap("left.bmp");
    iArrow[3] = al_load_bitmap("down.bmp");
    for(int i=0;i<4;i++){
        if (!iArrow[i]) {
                al_show_native_message_box(display, "Error", "Error",
                                           "Failed to load graphics!",
                                           nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                al_destroy_display(display);
                return -1;
        }
    }

    // Declare and initialize variables
    bool doexit = false;
    ALLEGRO_EVENT ev;

    int hiscore = 0, arrow=0,score=0,game=0,time=10*FPS;

    ifstream inFile("data.txt"); // Open and read in from the file
    if (inFile.is_open()) {
        inFile >> hiscore;
    }
    inFile.close();

	// Register timer events
    al_register_event_source(event_queue, al_get_display_event_source(display));
 	al_register_event_source(event_queue, al_get_keyboard_event_source());
 	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// Start the timer
	al_start_timer(timer);

	// Main game

	while (!doexit) {

        al_wait_for_event(event_queue, &ev);
        ALLEGRO_MOUSE_STATE state;
        al_get_mouse_state(&state);
        ALLEGRO_MOUSE_STATE mouse;
        al_get_mouse_state(&mouse);

        // Close the game
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
         	if (ev.keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
                doexit = true;
         	}
      	}

        // Update the screen

      	else if (ev.type == ALLEGRO_EVENT_TIMER) {

            al_clear_to_color(WHITE);

            al_draw_text(font,BLACK,disp_data.width/2,0,ALLEGRO_ALIGN_CENTER,"Arrow Game");
            al_draw_textf(font,BLACK,disp_data.width/2,7*disp_data.height/10,ALLEGRO_ALIGN_CENTER,"Time: %.2f",(float)time/FPS);
            al_draw_textf(font,BLACK,disp_data.width/2,4*disp_data.height/5,ALLEGRO_ALIGN_CENTER,"Score: %d",score);
            al_draw_textf(font,BLACK,disp_data.width/2,9*disp_data.height/10,ALLEGRO_ALIGN_CENTER,"Hi-score: %d",hiscore);
            al_draw_scaled_bitmap(iArrow[arrow], 0, 0,
                                  al_get_bitmap_width(iArrow[arrow]),
                                  al_get_bitmap_height(iArrow[arrow]),
                                  disp_data.width/3,disp_data.height/20*3,
                                  disp_data.width/3,disp_data.height/2,0);

            if(game){
                time--;
            }
            if(!time){
                game=0;
                time=10*FPS;
                if(score>hiscore){
                    hiscore=score;
                }
                score=0;
                arrow=0;
            }

      	}

      	// On key press

      	if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if(!game){
                game=1;
            }
            if(!arrow){
                if(ev.keyboard.keycode==ALLEGRO_KEY_RIGHT){
                    arrow=rand()%4;
                    score++;
                }
                else{
                    game=0;
                    time=10*FPS;
                    if(score>hiscore){
                        hiscore=score;
                    }
                    score=0;
                    arrow=0;
                }
            }
            else if(arrow==1){
                if(ev.keyboard.keycode==ALLEGRO_KEY_UP){
                    arrow=rand()%4;
                    score++;
                }
                else{
                    game=0;
                    time=10*FPS;
                    if(score>hiscore){
                        hiscore=score;
                    }
                    score=0;
                    arrow=0;
                }
            }
            else if(arrow==2){
                if(ev.keyboard.keycode==ALLEGRO_KEY_LEFT){
                    arrow=rand()%4;
                    score++;
                }
                else{
                    game=0;
                    time=10*FPS;
                    if(score>hiscore){
                        hiscore=score;
                    }
                    score=0;
                    arrow=0;
                }
            }
            else {
                if(ev.keyboard.keycode==ALLEGRO_KEY_DOWN){
                    arrow=rand()%4;
                    score++;
                }
                else{
                    game=0;
                    time=10*FPS;
                    if(score>hiscore){
                        hiscore=score;
                    }
                    score=0;
                    arrow=0;
                }
            }
      	}

      	al_flip_display();

	}

	al_destroy_display(display);
    return 0;

}
