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

#include "congfx.h"

uinteger x = 0;
uinteger y = 0;

typedef struct
{
	vec2 position;
	vec2 velocity;
	number colour;
} ball;

ball *make_ball()
{
	ball *b = (ball *)calloc(1, sizeof(ball));
	if (b == NULL)
	{
		wprintf(L"Unable to allocate ball struct.\n");
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

integer NUM_BALLS = 10;
ball **balls;

void setup()
{
	ball *_b = NULL;
	balls = (ball**)calloc(NUM_BALLS, sizeof(ball*));
	create_canvas(100, 50);

	for (integer i = 0; i < NUM_BALLS; i++)
	{
		_b = make_ball();
		int x = rand_number(0, width);
		int y = rand_number(0, height);
		wprintf(L"%d, %d\n", x, y);
		_b->position = make_vec2(x, y);
		_b->velocity = make_vec2(0.00001 * rand_number(-10, 10), 0.00001 * rand_number(-10, 10));
		balls[i] = _b;
	}
	for (integer i = 0; i < NUM_BALLS; i++)
	{
		wprintf(L"ball %lu at %Lf, %Lf\n", i, balls[i]->position[0], balls[i]->position[1]);
	}
	// setup() is run once at startup
	frame_rate(60);
	// no_loop();
}

void ball_update(ball *b, uinteger dt)
{
	vec2 vToAdd = vec2_mult_scalar(b->velocity, dt);
	vec2 newPos = vec2_add(
		b->position,
		vToAdd);
	dispose_vec2(b->position);
	dispose_vec2(vToAdd);
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
	// point(
	// 	(uinteger)(b->position[0]),
	// 	(uinteger)(b->position[1]),
	// 	L'█');
	rect(
		number_to_uinteger(b->position[0] - 2),
		number_to_uinteger(b->position[1] - 2),
		4, 4);
}

string calc_fps(uinteger dt)
{
	uinteger dt_millis = dt / 1000;
	uinteger fps = (1000.0 / dt_millis);
	string fps_str = make_string(100);
	swprintf(fps_str, 100, L"FPS: %lu", fps);
	return fps_str;
}

void draw(uinteger dt)
{
	// draw() is run multiple times per second.

	//set background colour
	//background(2);

	text(L"CONGFX: EXPERIMENT IN C ", width / 2 - 12, height / 2);

	// show fps
	string fps_str = calc_fps(dt);
	text(fps_str, 0, 0);

	// show ball pos
	// string ballpos_str = vec2_to_string(b->position);
	// text(ballpos_str, 0, 1);
	// dispose_string(ballpos_str);

	for (integer i = 0; i < NUM_BALLS; i++)
	{
		ball *a = balls[i];
		ball_show(a);
		ball_update(a, dt);
	}
	// // set point 0,0 to A
	// point(0, 0, L'A');

	// // set point 3,4 to B
	// point(3, 4, L'B');

	// // set point x, y to C
	// point(x, y, L'C');

	// if (y == height)
	// {
	// 	y = 0;
	// }
	// else
	// {
	// 	y += 1;
	// }

	// // a couple of lines
	// line(20, 20, 20, 25);
	// line(10, 10, 20, 20);

	// // a rectangle
	// rect(5, 5, 10, 10);

	// no_loop(); // no_loop stops the draw loop.
	dispose_string(fps_str);
}
