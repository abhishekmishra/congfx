#include "congfx.h"

cg_uint total_time = 0;
cg_uint step_time = 50000;
cg_uint radius = 1;

void setup()
{
    // does nothing
}

void draw(cg_uint dt)
{
    // determine the number of lines to draw
    // based on the total time elapsed
    // since the last times the number of lines to draw was updated
    if (total_time > step_time)
    {
        radius++;
        if (radius > height / 2)
        {
            radius = 1;
        }
        total_time = 0;
    }

    // draw the circle of radius `radius`
    cg_rect(width / 2 - radius, height / 2 - radius, 2 * radius, 2 * radius);

    // increment the total time
    total_time += dt;
}

void key_pressed(char c)
{
}