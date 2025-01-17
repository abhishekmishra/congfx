#include "congfx.h"

cg_uint total_time = 0;
cg_uint step_time = 50000;
cg_uint radius = 1;

void setup()
{
}

void draw(cg_uint dt)
{
    cg_rgb_t col = {0, 0, dt % 256};
    stroke(col);

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
    rect(width / 2 - radius, height / 2 - radius, 2 * radius, 2 * radius);

    // increment the total time
    total_time += dt;
}