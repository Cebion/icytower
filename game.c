#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <time.h>

#include "game.h"
#include "gfx.h"
#include "sfx.h"
#include "options.h"
#include "characters.h"
#include "floor_types.h"
#include "physics.h"

IT_STATE it_state;
int keys;

enum {
	ANIMATION_IDLE,
	ANIMATION_WALK_LEFT,
	ANIMATION_WALK_RIGHT,
	ANIMATION_ROTATE
} animation;

int animation_frame;

void initialize_game(void) {
	init_state(&it_state, rejump, time(NULL));
	keys = 0;
	animation = ANIMATION_IDLE;
	animation_frame = 0;
}

void press_left(void) { keys |= KEY_LEFT; }
void press_right(void) { keys |= KEY_RIGHT; }
void press_jump(void) { keys |= KEY_JUMP; }
void release_left(void) { keys &= ~KEY_LEFT; }
void release_right(void) { keys &= ~KEY_RIGHT; }
void release_jump(void) { keys &= ~KEY_JUMP; }

void do_tick(void) {
	play_frame(&it_state, keys);
	++animation_frame;
}

void draw_background(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));
}

void draw_floor(FLOOR *floor, unsigned int n) {
	int x, y = it_state.screen_y + 426 - 80 * n;
	unsigned int index = start_floor + n / 100;
	const struct floor_type *type;
	if (index >= floor_types_count)
		index = floor_types_count - 1;
	type = &floor_types[index];
	x = (floor->start + 1) * 16;
	al_draw_bitmap(type->left,
			x - al_get_bitmap_width(type->left), y, 0);
	while (x < floor->end * 16) {
		al_draw_bitmap(type->mid, x, y, 0);
		x += 16;
	}
	al_draw_bitmap(type->right, x, y, 0);
	if (n > 0 && n % 10 == 0) {
		x = (floor->start + floor->end) * 8;
		y += 16;
		al_draw_bitmap(type->sign, x, y, 0);
		x += al_get_bitmap_width(type->sign) / 2;
		y += 6;
		al_draw_textf(font_native, al_map_rgb(0, 0, 0), x - 1, y,
				ALLEGRO_ALIGN_CENTRE, "%u", n);
		al_draw_textf(font_native, al_map_rgb(0, 0, 0), x + 1, y,
				ALLEGRO_ALIGN_CENTRE, "%u", n);
		al_draw_textf(font_native, al_map_rgb(0, 0, 0), x, y - 1,
				ALLEGRO_ALIGN_CENTRE, "%u", n);
		al_draw_textf(font_native, al_map_rgb(0, 0, 0), x, y + 1,
				ALLEGRO_ALIGN_CENTRE, "%u", n);
		al_draw_textf(font_native, al_map_rgb(255, 255, 255), x, y,
				ALLEGRO_ALIGN_CENTRE, "%u", n);
	}
}

void draw_floors(void) {
	unsigned int i;
	FLOORS *floors = &it_state.floors;
	for (i = 1; i <= floors->count; ++i) {
		if (i > 7)
			break;
		draw_floor(&floors->floor[(floors->start - i + 7) % 7],
				floors->count - i);
	}
}

void draw_character(void) {
	switch (animation) {
	case ANIMATION_IDLE:
		switch ((animation_frame / 13) % 4) {
		case 0:
		case 2:
			al_draw_bitmap(characters[character_index].gfx.idle1,
			it_state.x - 14, it_state.y - 51, 0);
			break;
		case 1:
			al_draw_bitmap(characters[character_index].gfx.idle2,
			it_state.x - 14, it_state.y - 51, 0);
			break;
		case 3:
			al_draw_bitmap(characters[character_index].gfx.idle3,
			it_state.x - 14, it_state.y - 51, 0);
			break;
		}
		break;
	}
}

void draw_game(void) {
	if (fullscreen)
		al_clear_to_color(al_map_rgb(0, 0, 0));
	draw_background();
	draw_floors();
	draw_character();
}
