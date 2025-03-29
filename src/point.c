#include <allegro5/allegro5.h>
#include <float.h>
#include <math.h>
#include "lines.h"

void point_add_f(POINT* point, float x, float y)
{
	point->x += x;
	point->y += y;
}

void point_subtract_f(POINT* point, float x, float y)
{
	point->x -= x;
	point->y -= y;
}

void point_multiply_f(POINT* point, float x, float y)
{
	point->x *= x;
	point->y *= y;
}

void point_divide_f(POINT* point, float x, float y)
{
	ALLEGRO_ASSERT(x != 0.0f || y != 0.0f);

	point->x /= x;
	point->y /= y;
}

static bool float_equals(float a, float b)
{
	return fabs(a - b) < FLT_EPSILON;
}
	
bool point_equals_f(POINT* point, float x, float y)
{
	return float_equals(point->x, x) && float_equals(point->y, y);
}

void point_set_f(POINT* point, float x, float y)
{
	point->x = x;
	point->y = y;
}

void point_add(POINT* point, const POINT* point2)
{
	point->x += point2->x;
	point->y += point2->y;
}

void point_subtract(POINT* point, const POINT* point2)
{
	point->x -= point2->x;
	point->y -= point2->y;
}

void point_multiply(POINT* point, const POINT* point2)
{
	point->x *= point2->x;
	point->y *= point2->y;
}

void point_divide(POINT* point, const POINT* point2)
{
	ALLEGRO_ASSERT(point2->x != 0.0f || point2->y != 0.0f);

	point->x /= point2->x;
	point->y /= point2->y;
}

bool point_equals(const POINT* point, const POINT* point2)
{
	return float_equals(point->x, point2->x) && float_equals(point->y, point2->y);
}

void point_set(POINT* point, const POINT* point2)
{
	point->x = point2->x;
	point->y = point2->y;
}

void point_set_zero(POINT* point)
{
	point->x = 0.0f;
	point->y = 0.0f;
}

void point_get_difference(POINT* result, const POINT* point, const POINT* point2)
{
	result->x = point->x - point2->x;
	result->y = point->y - point2->y;
}

void point_invert(POINT* point)
{
	float temp = point->x;
	point->x = point->y;
	point->y = temp;
}
