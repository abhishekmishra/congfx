#include "piecer.h"

void setup()
{
	// setup() is run once at startup
}

size_t x1 = 0, y1 = 0;

void draw()
{
	// draw() is run multiple times per second.

	// set background character
	background(L' ');

	// set point 0,0 to A
	point(0, 0, L'A');

	// set point 3,3 to B
	point(3, 4, L'B');

	// set point x1, y1 to C
	point(x1, y1, L'C');

	if (y1 == height)
	{
		y1 = 0;
	}
	else
	{
		y1 += 1;
	}

	// noLoop(); // noLoop stops the draw loop.
}