#include <allegro5/allegro.h>

#include "fullscreen.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))

void enable_fullscreen(void) {
    ALLEGRO_DISPLAY *display = al_get_current_display();
    int width, height;
    float scale_x, scale_y;
    ALLEGRO_TRANSFORM transform;

    al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);
    width = al_get_display_width(display);
    height = al_get_display_height(display);

    // Calculate scaling factors
    scale_x = width / 640.0f;
    scale_y = height / 480.0f;

    // Create a transformation that centers and scales the game
    al_identity_transform(&transform);
    al_scale_transform(&transform, scale_x, scale_y);
    al_translate_transform(&transform, (width - 640 * scale_x) / 2, (height - 480 * scale_y) / 2);
    al_use_transform(&transform);

    // Clear the screen
    al_clear_to_color(al_map_rgb(0, 0, 0));
}

void disable_fullscreen(void) {
    ALLEGRO_DISPLAY *display = al_get_current_display();
    ALLEGRO_TRANSFORM transform;

    al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, false);
    
    // Reset the transform
    al_identity_transform(&transform);
    al_use_transform(&transform);
}