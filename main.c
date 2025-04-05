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
static ALLEGRO_BITMAP* cursor = NULL;
static MOUSE mouse = { { 0.0f, 0.0f}, false, 0, 0 };
static TRIANGLE triangle = { {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f} };
static bool redraw = true;
static bool running = true;
static int line_count = 1;
static BUTTON key[ALLEGRO_KEY_MAX];
static POINT mouse_diff = { 0.0f, 0.0f };
static POINT display_size = { 0.0f, 0.0f };
static POINT bitmap_size = { 0.0f, 0.0f };
static POINT aspect_ratio = { 0.0f, 0.0f };

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
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!display)
	{
		return -1;
	}
	al_clear_to_color(EIGENGRAU);
	al_flip_display();
	point_set_f(&display_size, (float)al_get_display_width(display), (float)al_get_display_height(display));
	al_hide_mouse_cursor(display);

	cursor = create_mouse_cursor();
	if (!cursor)
	{
		return -1;
	}

	bitmap = al_create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!bitmap)
	{
		return -1;
	}

	ALLEGRO_BITMAP* target_bitmap = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);
	al_clear_to_color(EIGENGRAU);
	al_set_target_bitmap(target_bitmap);

	point_set_f(&bitmap_size, (float)al_get_bitmap_width(bitmap), (float)al_get_bitmap_height(bitmap));
	point_set(&aspect_ratio, &display_size);
	point_divide(&aspect_ratio, &bitmap_size);

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
	if (bitmap)
	{
		al_destroy_bitmap(bitmap);
	}
	if (cursor)
	{
		al_destroy_bitmap(cursor);
	}
	if (event_queue)
	{
		al_destroy_event_queue(event_queue);
	}
	if (timer)
	{
		al_destroy_timer(timer);
	}
	if (display)
	{
		al_destroy_display(display);
	}
}

static void process_input(int keycode)
{
	static bool line_change = false;

	switch (keycode)
	{
	case ALLEGRO_KEY_ESCAPE:
	{
		running = false;
	} break;
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
	case ALLEGRO_KEY_TAB:
	{
		point_set(&mouse.point, &triangle.point1);
		point_set(&triangle.point1, &triangle.point2);
		point_set(&triangle.point2, &mouse.point);
		al_set_mouse_xy(display, (int)mouse.point.x, (int)mouse.point.y);
	} break;
	default: return;
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
		case ALLEGRO_EVENT_DISPLAY_RESIZE:
		{
			al_acknowledge_resize(display);
			point_set_f(&display_size, (float)al_get_display_width(display), (float)al_get_display_height(display));
			point_set(&aspect_ratio, &display_size);
			point_divide(&aspect_ratio, &bitmap_size);
		} break;
		case ALLEGRO_EVENT_KEY_DOWN:
		{
			key[event.keyboard.keycode].is_pressed = true;
			key[event.keyboard.keycode].was_pressed = true;
		} break;
		case ALLEGRO_EVENT_KEY_UP:
		{
			key[event.keyboard.keycode].is_pressed = false;

			process_input(event.keyboard.keycode);

		} break;
		case ALLEGRO_EVENT_MOUSE_AXES:
		{
			static POINT event_mouse = { 0.0f, 0.0f };

			point_set_f(&event_mouse, (float)event.mouse.x, (float)event.mouse.y);
			point_set_f(&mouse_diff, (float)event.mouse.dx, (float)event.mouse.dy);

			point_multiply(&event_mouse, &aspect_ratio);
			point_multiply(&mouse_diff, &aspect_ratio);

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
		default: return;
		}
	}
}

static void logic()
{
	if (key[ALLEGRO_KEY_LSHIFT].is_pressed)
	{
		if (key[ALLEGRO_KEY_LSHIFT].was_pressed)
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
	static POINT line_start = { 0.0f, 0.0f };
	static POINT line_end = { 0.0f, 0.0f };

	target_bitmap = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);

	if (key[ALLEGRO_KEY_LSHIFT].is_pressed)
	{
		draw_triangles();
	}

	al_set_target_bitmap(target_bitmap);
	al_draw_scaled_bitmap(bitmap, 0, 0, bitmap_size.x, bitmap_size.y, 0.0f, 0.0f, display_size.x, display_size.y, 0);
	if (key[ALLEGRO_KEY_LSHIFT].is_pressed && (mouse.is_pressed & ALLEGRO_MOUSE_BUTTON_LEFT))
	{
		point_set(&line_start, &triangle.point1);
		point_set(&line_end, &triangle.point2);
		point_divide(&line_start, &bitmap_size);
		point_multiply(&line_start, &display_size);
		point_divide(&line_end, &bitmap_size);
		point_multiply(&line_end, &display_size);
		draw_line(&line_start, &line_end, &RED, 2.0f);
	}
	draw_mouse_cursor(cursor, &mouse.point);
	al_flip_display();
}

static void loop()
{
	while (running)
	{
		for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
		{
			key[i].was_pressed = false;
		}
		mouse.was_pressed = 0;

		input();
		logic();
		draw();
	}
}

int main(int argc, char** argv)
{
	if (init() == 0)
	{
		al_start_timer(timer);
		loop();
		al_stop_timer(timer);
	}

	shutdown();

	return 0;
}
