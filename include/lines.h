#ifndef _LINES_H
#define _LINES_H

#include <allegro5/allegro.h>
#include <stdint.h>

static const int SCREEN_WIDTH = 1400;
static const int SCREEN_HEIGHT = 800;
static const int SHADE_MAX = 360;
static const int SHADE_DELTA = 2;
static const float MARGIN = 0.2f;

enum COLOR_INDEX
{
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_CYAN,
	COLOR_RED,
	COLOR_PURPLE,
	COLOR_YELLOW,
	COLOR_WHITE,
	COLOR_COUNT
};

// RGB color struct
typedef struct COLOR
{
	float r;
	float g;
	float b;
} COLOR;

typedef struct POINT
{
	float x;
	float y;
} POINT;

typedef struct TRIANGLE
{
	POINT point1;
	POINT point2;
	POINT point3;
} TRIANGLE;

typedef struct MOUSE
{
	POINT point;
	bool moved;
	uint32_t is_pressed;
	uint32_t was_pressed;
} MOUSE;

// Point functions
void point_add_f(POINT* point, float x, float y);
void point_subtract_f(POINT* point, float x, float y);
void point_multiply_f(POINT* point, float x, float y);
void point_divide_f(POINT* point, float x, float y);
bool point_equals_f(POINT* point, float x, float y);
void point_set_f(POINT* point, float x, float y);
void point_add(POINT* point, const POINT* point2);
void point_subtract(POINT* point, const POINT* point2);
void point_multiply(POINT* point, const POINT* point2);
void point_divide(POINT* point, const POINT* point2);
bool point_equals(const POINT* point, const POINT* point2);
void point_set(POINT* point, const POINT* point2);
void point_set_zero(POINT* point);
void point_get_difference(POINT* result, const POINT* point, const POINT* point2);
void point_invert(POINT* point);

// Color functions
void color_set(COLOR* color, float r, float g, float b);
int get_color_index();
void adjust_color_index(int delta);
float calculate_shade();
void get_color(int index, COLOR* color);

// Draw functions
void draw_triangle_p(const POINT* point1, const POINT* point2, const POINT* point3, const COLOR* color, float thickness);
void draw_filled_triangle_p(const POINT* point1, const POINT* point2, const POINT* point3, const COLOR* color);
void draw_triangle(const TRIANGLE* triangle, const COLOR* color, float thickness);
void draw_filled_triangle(const TRIANGLE* triangle, const COLOR* color);
void draw_line(const POINT* point1, const POINT* point2, const COLOR* color, float thickness);


#endif // !_LINES_H
