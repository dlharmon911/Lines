#include <allegro5/allegro.h>
#include "lines.h"

static int _color_index = 0;

void color_set(COLOR* color, float r, float g, float b)
{
	color->r = r;
	color->g = g;
	color->b = b;
}

void adjust_color_index(int delta)
{
	if (delta == 0)
	{
		_color_index = 0;
	}
	else
	{
		_color_index = _color_index + delta;

		if (_color_index < 0)
		{
			_color_index = (COLOR_COUNT - 1);
		}

		if (_color_index >= COLOR_COUNT)
		{
			_color_index = 0;
		}
	}
}

int get_color_index()
{
	return _color_index;
}

float calculate_shade()
{
	static int shade = 0;
	static int direction = 1;

	shade = shade + SHADE_DELTA * direction;

	if (shade < 0)
	{
		shade = shade + SHADE_DELTA;
		direction = 1;
	}

	if (shade >= SHADE_MAX)
	{
		adjust_color_index(1);
		shade = shade - SHADE_DELTA;
		direction = -1;
	}

	return 1.0f - (((float)shade / (float)SHADE_MAX) * (1.0f - (2.0f * MARGIN)) + MARGIN);
}

void get_color(int index, COLOR* color)
{
	static const COLOR color_list[COLOR_COUNT] =
	{
		{0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 0.0f},
		{1.0f, 1.0f, 1.0f}
	};

	color->r = color_list[index].r;
	color->g = color_list[index].g;
	color->b = color_list[index].b;
}


