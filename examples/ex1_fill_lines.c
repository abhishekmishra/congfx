#include "piecer.h"

uinteger total_time = 0;
uinteger step_time = 50000;
character total_time_string[1024];
uinteger lines_to_draw = 0;

void setup()
{
}

void draw(uinteger dt)
{
    // determine the number of lines to draw
    // based on the total time elapsed
    // since the last times the number of lines to draw was updated
    if (total_time > step_time)
    {
        lines_to_draw++;
        if (lines_to_draw > height)
        {
            lines_to_draw = 0;
        }
        total_time = 0;
    }

    // draw the lines
    for (uinteger i = 0; i < lines_to_draw; i++)
    {
        line(0, i, width, i);
    }

    // draw the total time in the center of the canvas
    swprintf(total_time_string, 1024, L"Total time: %5lu", total_time);
    text(total_time_string, (width - wcslen(total_time_string))/2, 10);
    total_time += dt;
}