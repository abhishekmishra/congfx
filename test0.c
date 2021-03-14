#include "piecer.h"

uinteger x = 0;
uinteger y = 0;

typedef struct
{
	vec2 position;
	vec2 velocity;
} ball;

ball *makeBall()
{
	ball *b = (ball *)calloc(1, sizeof(ball));
	if (b == NULL)
	{
		wprintf(L"Unable to allocate ball struct.\n");
		exit(-1);
	}
	return b;
}

void disposeBall(ball *b)
{
	if (b == NULL)
	{
		free(b);
	}
}

ball *b = NULL;
integer NUM_BALLS = 5;
ball **balls;

void setup()
{
	balls = *ball[NUM_BALLS];
	for (integer i = 0; i < NUM_BALLS; i++)
	{
		b = makeBall();
		b->position = makeVec2(20, 10);
		b->velocity = makeVec2(0.00001, 0.00001);
		balls[i] = b;
	}

	// setup() is run once at startup
	frameRate(25);
}

void ball_update(uinteger dt)
{
	vec2 vToAdd = vec2MultScalar(b->velocity, dt);
	vec2 newPos = vec2Add(
		b->position,
		vToAdd);
	disposeVec2(b->position);
	disposeVec2(vToAdd);
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

string calc_fps(uinteger dt)
{
	uinteger dt_millis = dt / 1000;
	uinteger fps = (1000.0 / dt_millis);
	string fps_str = makeString(100);
	swprintf(fps_str, 100, L"FPS: %lu", fps);
	return fps_str;
}

void draw(uinteger dt)
{
	// draw() is run multiple times per second.

	//set background character
	background(L' ');

	text(L"PIECER: EXPERIMENT IN C ", width / 2 - 12, height / 2);

	// show fps
	string fps_str = calc_fps(dt);
	text(fps_str, 0, 0);

	// show ball pos
	string ballpos_str = vec2ToString(b->position);
	text(ballpos_str, 0, 1);
	disposeString(ballpos_str);

	point(
		(uinteger)(b->position[0]),
		(uinteger)(b->position[1]),
		L'█');
	ball_update(dt);

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

	// noLoop(); // noLoop stops the draw loop.
	disposeString(fps_str);
}
