#include "piecer.h"

void setup()
{
	// setup() is run once at startup
}

size_t x = 0;
size_t y = 0;

void draw(long dt)
{
	// draw() is run multiple times per second.

	//set background character
	background(L' ');

	// set point 0,0 to A
	point(0, 0, L'A');

	// set point 3,4 to B
	point(3, 4, L'B');

	// set point x, y to C
	point(x, y, L'C');

	if (y == height)
	{
		y = 0;
	}
	else
	{
		y += 1;
	}

	// a couple of lines
	line(20, 20, 20, 25);
	line(10, 10, 20, 20);

	// a rectangle
	rect(5, 5, 10, 10);

	//noLoop(); // noLoop stops the draw loop.
}
