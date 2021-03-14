#include "piecer.h"

uinteger x = 0;
uinteger y = 0;

//ball position
double ballx = 20;
double bally = 10;

//ball velocity
double balldx = 0.00001;
double balldy = 0.00001;

void setup()
{
	// setup() is run once at startup
	frameRate(25);
}

void ball_update(uinteger dt)
{
	ballx = ballx + (balldx * dt);
	bally = bally + (balldy * dt);
	if (ballx > width || ballx < 0)
	{
		balldx = balldx * -1.0;
	}
	if (bally > height || bally < 0)
	{
		balldy = balldy * -1.0;
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

	// show fps
	string fps_str = calc_fps(dt);
	text(fps_str, 0, 0);

	point(ballx, bally, L'X');
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
