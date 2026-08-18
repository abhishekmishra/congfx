/*
Copyright 2021-25 Abhishek Mishra <neolateral.in>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#define CONGFX_IMPLEMENTATION
#include "congfx.h"

/*+++++++++ BEGIN Vec2 TYPE FUNCTIONS +++++++++*/

/**
 * Create a new vector of 2 elements.
 *
 * @param v1 The first element of the vector.
 * @param v2 The second element of the vector.
 * @return The new vector.
 */
vec2 cg_make_vec2(cg_number v1, cg_number v2);

/**
 * Create a new vector of 2 elements from another vector.
 *
 * @param other The vector to copy from.
 * @return The new vector.
 */
vec2 cg_make_vec2_from(vec2 other);

/**
 * Add two vectors.
 *
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return The sum of the two vectors.
 */
vec2 cg_vec2_add(vec2 v1, vec2 v2);

/**
 * Multiply a vector by a scalar.
 *
 * @param vin The vector to multiply.
 * @param x The scalar to multiply by.
 * @return The product of the vector and the scalar.
 */
vec2 cg_vec2_mult_scalar(vec2 vin, cg_number x);

/**
 * Convert a vector to a string.
 *
 * @param v The vector to convert.
 * @return The string representation of the vector.
 */
cg_string cg_vec2_to_string(vec2 v);

/**
 * Dispose of a vector.
 *
 * @param v The vector to dispose of.
 */
void cg_dispose_vec2(vec2 v);

vec2 cg_make_vec2(cg_number v0, cg_number v1)
{
	vec2 v = (vec2)_CG_CALLOC(2, sizeof(cg_number));
	if (v == NULL)
	{
		printf("FATAL Error: Unable to allocate vec2.\n");
		exit(-1);
	}
	v[0] = v0;
	v[1] = v1;
	return v;
}

vec2 cg_make_vec2_from(vec2 other)
{
	vec2 v = cg_make_vec2(other[0], other[1]);
	return v;
}

vec2 cg_vec2_add(vec2 v1, vec2 v2)
{
	vec2 res = cg_make_vec2_from(v1);
	res[0] = res[0] + v2[0];
	res[1] = res[1] + v2[1];
	return res;
}

vec2 cg_vec2_mult_scalar(vec2 vin, cg_number x)
{
	vec2 v = cg_make_vec2_from(vin);
	v[0] = v[0] * x;
	v[1] = v[1] * x;
	return v;
}

cg_string cg_vec2_to_string(vec2 v)
{
	cg_string s = cg_make_string(100);
	snprintf(s, 100, "%.2Lf, %.2Lf", v[0], v[1]);
	return s;
}

void cg_dispose_vec2(vec2 v)
{
	if (v != NULL)
	{
		_CG_FREE(v);
	}
}

/*+++++++++ END Vec2 TYPE FUNCTIONS +++++++++*/

#define NUM_BALLS 10
#define FPS_STR_LEN 100

cg_uint x = 0;
cg_uint y = 0;
cg_char fps_str[FPS_STR_LEN];

typedef struct
{
	vec2 position;
	vec2 velocity;
	cg_number colour;
} ball;

ball *make_ball();
void dispose_ball(ball *b);
void ball_update(ball *b, cg_uint dt);
void ball_show(ball *b);
cg_string calc_fps(cg_uint dt);

int main(int argc, char *argv[])
{
	ball *_b = NULL;
	ball **balls;

	// create the graphics engine
	int err = cg_create_graphics_fullscreen();
	if (err != 0)
	{
		return err;
	}

	// initialize the balls
	balls = (ball **)calloc(NUM_BALLS, sizeof(ball *));
	if (balls == NULL)
	{
		printf("Unable to allocate balls array.\n");
		exit(-1);
	}

	for (cg_int i = 0; i < NUM_BALLS; i++)
	{
		_b = make_ball();
		int x = cg_rand_int(0, width);
		int y = cg_rand_int(0, height);
		// wprintf(L"%d, %d\n", x, y);
		_b->position = cg_make_vec2(x, y);
		_b->velocity = cg_make_vec2(cg_rand_int(15, 25), cg_rand_int(15, 25));
		_b->velocity = cg_vec2_mult_scalar(_b->velocity, 0.001);

		// get a random sign for x and y
		if (cg_rand_int(0, 1) < 0.5)
		{
			_b->velocity[0] = _b->velocity[0] * -1.0;
		}
		if (cg_rand_int(0, 1) < 0.5)
		{
			_b->velocity[1] = _b->velocity[1] * -1.0;
		}
		balls[i] = _b;
	}

	// set the frame rate
	cg_frame_rate(60);

	while (!cg_should_exit())
	{
		// begin the draw
		cg_begin_draw();

		// clear canvas before every frame
		cg_clear_canvas();

		// show and update all the balls
		for (cg_int i = 0; i < NUM_BALLS; i++)
		{
			ball *a = balls[i];
			ball_show(a);
			ball_update(a, cg_get_deltatime());
		}

		// print title
		cg_text("CONGFX: EXAMPLE#5: BOUNCING BALLS ", width / 2 - 17, 0);

		// show fps
		cg_string fps_str = calc_fps(cg_get_deltatime());
		cg_text(fps_str, 0, 0);

		// print press escape to exit
		cg_text("Press ESC to exit", 0, height - 1);

		// end the draw
		cg_end_draw();
	}

	// destroy the graphics engine
	cg_destroy_graphics();
}

ball *make_ball()
{
	ball *b = (ball *)calloc(1, sizeof(ball));
	if (b == NULL)
	{
		printf("Unable to allocate ball struct.\n");
		exit(-1);
	}
	b->colour = 15;
	return b;
}

void dispose_ball(ball *b)
{
	if (b == NULL)
	{
		free(b);
	}
}

void ball_update(ball *b, cg_uint dt)
{
	vec2 vToAdd = cg_vec2_mult_scalar(b->velocity, dt);
	vec2 newPos = cg_vec2_add(
		b->position,
		vToAdd);
	cg_dispose_vec2(b->position);
	cg_dispose_vec2(vToAdd);
	b->position = newPos;

	if (b->position[0] > width || b->position[0] < 0)
	{
		b->velocity[0] = b->velocity[0] * -1.0;
	}
	if (b->position[1] > height || b->position[1] < 0)
	{
		b->velocity[1] = b->velocity[1] * -1.0;
	}
}

void ball_show(ball *b)
{
	cg_rect(
		cg_number_to_uinteger(b->position[0] - 2),
		cg_number_to_uinteger(b->position[1] - 2),
		4, 4);
}

cg_string calc_fps(cg_uint dt_millis)
{
	cg_uint fps = (1000.0 / dt_millis);
	snprintf(fps_str, FPS_STR_LEN, "FPS: %lu", fps);
	return fps_str;
}