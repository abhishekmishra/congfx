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

/**
 * @file ex1_fill_lines.c
 * @author Abhishek Mishra
 * @brief Example 1: Fill lines on the canvas
 * @version 0.1
 * @date 2025-01-15
 * 
 * @copyright Copyright (c) 2025
 * 
 * This example demonstrates a simple animation
 * on the canvas. We slowly fill the canvas with lines
 * starting from the top and moving downwards.
 * 
 * Each new line is added after a certain time interval.
 * This interval is controlled by the `step_time` variable.
 * 
 * Once all lines are drawn, the canvas is cleared and the
 * process starts again.
 * 
 * To exit the program, press `Ctrl+C`.
 */

#include "congfx.h"

uinteger total_time = 0;
uinteger step_time = 50000;
character total_time_string[1024];
uinteger lines_to_draw = 0;

void setup()
{
    // does nothing but is required
}

/**
 * @brief Increment the number of lines to draw
 * everty `step_time` microseconds.
 * 
 * @param dt delta time in microseconds
 */
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