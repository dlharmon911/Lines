#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "lines.h"

static ALLEGRO_DISPLAY* display = NULL;
static ALLEGRO_EVENT_QUEUE* event_queue = NULL;
static ALLEGRO_TIMER* timer = NULL;
static ALLEGRO_BITMAP* bitmap = NULL;
static MOUSE mouse = { { 0.0f, 0.0f}, false, 0, 0 };
static TRIANGLE triangle = { {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f} };
static bool redraw = true;
static bool running = true;
static int line_count = 1;
static ALLEGRO_COLOR EIGENGRAU = { 0.08627451f, 0.08627451f, 0.11372549f, 1.0f };

static int init()
{
	if (!al_init())
	{
		return -1;
	}

	if (!al_init_primitives_addon())
	{
		return -1;
	}

	if (!al_install_mouse())
	{
		return -1;
	}

	if (!al_install_keyboard())
	{
		return -1;
	}

	timer = al_create_timer(1.0 / 60);
	if (!timer)
	{
		return -1;
	}

	al_set_new_window_title("Lines");
	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!display)
	{
		return -1;
	}
	al_clear_to_color(EIGENGRAU);
	al_flip_display();

	bitmap = al_create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!bitmap)
	{
		return -1;
	}

	ALLEGRO_BITMAP* target_bitmap = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);
	al_clear_to_color(EIGENGRAU);
	al_set_target_bitmap(target_bitmap);

	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	srand((unsigned int)time(NULL));

	return 0;
}

static void shutdown()
{
	al_destroy_bitmap(bitmap);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
}

static void process_input(int keycode)
{
	static bool line_change = false;

	switch (keycode)
	{
	case ALLEGRO_KEY_SPACE:
	{
		ALLEGRO_BITMAP* target_bitmap = al_get_target_bitmap();
		al_set_target_bitmap(bitmap);
		al_clear_to_color(EIGENGRAU);
		al_set_target_bitmap(target_bitmap);
	} break;
	case ALLEGRO_KEY_PGUP:
	{
		adjust_color_index(-1);
	} break;
	case ALLEGRO_KEY_PGDN:
	{
		adjust_color_index(-1);
	} break;
	case ALLEGRO_KEY_1:
	{
		if (line_count != 1)
		{
			line_change = true;
		}

		line_count = 1;
	} break;
	case ALLEGRO_KEY_2:
	{
		if (line_count != 2)
		{
			line_change = true;
		}

		line_count = 2;
	} break;
	case ALLEGRO_KEY_3:
	{
		if (line_count != 4)
		{
			line_change = true;
		}

		line_count = 4;
	} break;
	}

	if (line_change)
	{
		mouse.is_pressed &= (~((uint32_t)ALLEGRO_MOUSE_BUTTON_LEFT));
	}
}

static void input()
{
	static ALLEGRO_EVENT event;

	while (!al_event_queue_is_empty(event_queue))
	{
		al_get_next_event(event_queue, &event);

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
		{
			redraw = true;
		} break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
		{
			running = false;
		} break;
		case ALLEGRO_EVENT_KEY_UP:
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				running = false;
			}

			process_input(event.keyboard.keycode);

		} break;
		case ALLEGRO_EVENT_MOUSE_AXES:
		{
			static POINT event_mouse = { 0.0f, 0.0f };

			point_set_f(&event_mouse, (float)event.mouse.x, (float)event.mouse.y);

			if (!point_equals(&mouse.point, &event_mouse))
			{
				mouse.moved = true;
			}

			point_set(&mouse.point, &event_mouse);
		} break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		{
			mouse.was_pressed |= event.mouse.button;
			mouse.is_pressed |= event.mouse.button;
		} break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
		{
			mouse.is_pressed &= (~event.mouse.button);
		} break;
		}
	}
}

static void logic()
{
	if (mouse.is_pressed & ALLEGRO_MOUSE_BUTTON_LEFT)
	{
		if (mouse.was_pressed & ALLEGRO_MOUSE_BUTTON_LEFT)
		{
			point_set(&triangle.point1, &mouse.point);
			point_set(&triangle.point2, &mouse.point);
			point_set(&triangle.point3, &mouse.point);
		}

		if (mouse.moved)
		{
			point_set(&triangle.point3, &triangle.point2);
			point_set(&triangle.point2, &mouse.point);
		}
	}
}

static void draw_triangles()
{
	static TRIANGLE output = { {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f} };
	static POINT diff_point2 = { 0.0f, 0.0f };
	static POINT diff_point3 = { 0.0f, 0.0f };
	static COLOR rgba = { 0.0f, 0.0f, 0.0f };
	static float shade = 0.0f;
	static int color_index = 0;
	static COLOR color = { 0.0f, 0.0f, 0.0f };

	get_color(color_index, &color);
	color_index = get_color_index();
	shade = calculate_shade();

	rgba.r = (shade * color.r);
	rgba.g = (shade * color.g);
	rgba.b = (shade * color.b);

	point_set(&output.point1, &triangle.point1);
	point_set(&output.point2, &triangle.point2);
	point_set(&output.point3, &triangle.point3);
	draw_filled_triangle(&output, &rgba);

	point_set(&diff_point2, &output.point1);
	point_subtract(&diff_point2, &output.point2);
	point_set(&diff_point3, &output.point1);
	point_subtract(&diff_point3, &output.point3);

	if (line_count > 1)
	{
		point_set(&output.point2, &output.point1);
		point_add(&output.point2, &diff_point2);
		point_set(&output.point3, &output.point1);
		point_add(&output.point3, &diff_point3);
		draw_filled_triangle(&output, &rgba);

		if (line_count > 2)
		{
			point_set(&output.point2, &output.point1);
			point_add(&output.point2, &diff_point2);
			point_set(&output.point3, &output.point1);
			point_add(&output.point3, &diff_point3);
			draw_filled_triangle(&output, &rgba);
			
			point_invert(&diff_point2);
			point_invert(&diff_point3);
			point_multiply_f(&diff_point2, 1.0f, -1.0f);
			point_multiply_f(&diff_point3, 1.0f, -1.0f);

			point_set(&output.point2, &output.point1);
			point_subtract(&output.point2, &diff_point2);
			point_set(&output.point3, &output.point1);
			point_subtract(&output.point3, &diff_point3);
			draw_filled_triangle(&output, &rgba);

			point_set(&output.point2, &output.point1);
			point_add(&output.point2, &diff_point2);
			point_set(&output.point3, &output.point1);
			point_add(&output.point3, &diff_point3);
			draw_filled_triangle(&output, &rgba);
		}
	}
}

static void draw()
{
	static ALLEGRO_BITMAP* target_bitmap = NULL;
	static const COLOR RED = { 1.0f, 0.0f, 0.0f };

	target_bitmap = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);

	if (mouse.is_pressed & ALLEGRO_MOUSE_BUTTON_LEFT)
	{
		draw_triangles();
	}

	al_set_target_bitmap(target_bitmap);
	al_draw_bitmap(bitmap, 0, 0, 0);
	if (mouse.is_pressed & ALLEGRO_MOUSE_BUTTON_LEFT)
	{
		draw_line(&triangle.point1, &triangle.point2, &RED, 2.0f);
	}
	al_flip_display();
}

static void loop()
{
	while (running)
	{
		input();
		logic();
		mouse.was_pressed = 0;
		draw();
	}
}

int main(int argc, char** argv)
{
	if (init() != 0)
	{
		return -1;
	}

	al_start_timer(timer);

	loop();

	al_stop_timer(timer);

	shutdown();

	return 0;
}
