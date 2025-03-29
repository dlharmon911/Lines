#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "lines.h"

static ALLEGRO_COLOR color_to_allegro_color(const COLOR* color)
{
	return al_map_rgba_f(color->r, color->g, color->b, 1.0f);
}

void draw_triangle_p(const POINT* point1, const POINT* point2, const POINT* point3, const COLOR* color, float thickness)
{
	al_draw_triangle(point1->x, point1->y, point2->x, point2->y, point3->x, point3->y, color_to_allegro_color(color), thickness);
}

void draw_filled_triangle_p(const POINT* point1, const POINT* point2, const POINT* point3, const COLOR* color)
{
	al_draw_filled_triangle(point1->x, point1->y, point2->x, point2->y, point3->x, point3->y, color_to_allegro_color(color));
}

void draw_triangle(const TRIANGLE* triangle, const COLOR* color, float thickness)
{
	draw_triangle_p(&triangle->point1, &triangle->point2, &triangle->point3, color, thickness);
}

void draw_filled_triangle(const TRIANGLE* triangle, const COLOR* color)
{
	draw_filled_triangle_p(&triangle->point1, &triangle->point2, &triangle->point3, color);
}

void draw_line(const POINT* point1, const POINT* point2, const COLOR* color, float thickness)
{
	al_draw_line(point1->x, point1->y, point2->x, point2->y, color_to_allegro_color(color), thickness);
}
