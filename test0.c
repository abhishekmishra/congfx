#include "piecer.h"

void setup()
{
	// setup() is run once at startup
}

size_t x1 = 0;
size_t y1 = 0;

void draw()
{
	// draw() is run multiple times per second.

	//set background character
	background(L' ');

	// set point 0,0 to A
	point(0, 0, L'A');

	// set point 3,4 to B
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

	// a couple of lines
	line(20, 20, 20, 25);
	line(10, 10, 20, 20);

	// a rectangle
	rect(5, 5, 10, 10);

	// noLoop(); // noLoop stops the draw loop.
}