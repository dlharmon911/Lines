#include "lines.h"

#include <allegro5/allegro_primitives.h>

static const POINT cursor_size = { 16.0f, 16.0f };
static ALLEGRO_COLOR CURSOR_COLOR = { 0.85f, 0.85f, 0.85f, 0.6f };
static const POINT focus = { 7.5f, 7.5f };

ALLEGRO_BITMAP* create_mouse_cursor()
{
	ALLEGRO_BITMAP* cursor = al_create_bitmap((int)cursor_size.x, (int)cursor_size.y);
	ALLEGRO_BITMAP* target_bitmap = al_get_target_bitmap();
	if (!cursor)
	{
		return NULL;
	}
	al_set_target_bitmap(cursor);
	al_clear_to_color(al_map_rgb(255, 0, 255));

	al_draw_line(focus.x, 0.0f, focus.x, cursor_size.y, CURSOR_COLOR, 1.0f);
	al_draw_line(0.0f, focus.y, cursor_size.x, focus.y, CURSOR_COLOR, 1.0f);


	al_convert_mask_to_alpha(cursor, al_map_rgb(255, 0, 255));
	al_set_target_bitmap(target_bitmap);

	return cursor;
}

void draw_mouse_cursor(ALLEGRO_BITMAP* cursor, POINT* point)
{
	al_draw_bitmap(cursor, point->x - focus.x, point->y - focus.y, 0);
}
