#include "piecer.h"

uinteger x = 0;
uinteger y = 0;

typedef struct
{
	vec2 position;
	vec2 velocity;
	number colour;
} ball;

ball *makeBall()
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

void disposeBall(ball *b)
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
	createCanvas(100, 25);

	for (integer i = 0; i < NUM_BALLS; i++)
	{
		_b = makeBall();
		int x = randNumber(0, width);
		int y = randNumber(0, height);
		wprintf(L"%d, %d\n", x, y);
		_b->position = makeVec2(x, y);
		_b->velocity = makeVec2(0.00001 * randNumber(-10, 10), 0.00001 * randNumber(-10, 10));
		balls[i] = _b;
	}
	for (integer i = 0; i < NUM_BALLS; i++)
	{
		wprintf(L"ball %lu at %Lf, %Lf\n", i, balls[i]->position[0], balls[i]->position[1]);
	}
	// setup() is run once at startup
	frameRate(40);
	// noLoop();
}

void ball_update(ball *b, uinteger dt)
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

void ball_show(ball *b)
{
	point(
		(uinteger)(b->position[0]),
		(uinteger)(b->position[1]),
		L'█');
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

	//set background colour
	//background(2);

	text(L"PIECER: EXPERIMENT IN C ", width / 2 - 12, height / 2);

	// show fps
	string fps_str = calc_fps(dt);
	text(fps_str, 0, 0);

	// show ball pos
	// string ballpos_str = vec2ToString(b->position);
	// text(ballpos_str, 0, 1);
	// disposeString(ballpos_str);

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

	// noLoop(); // noLoop stops the draw loop.
	disposeString(fps_str);
}
