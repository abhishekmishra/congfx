#include "congfx.h"

cg_rgb_t fg_colour = {255, 100, 0};
cg_rgb_t bg_colour = {0, 100, 0};

void setup()
{
    // background(12);
    // stroke(15);
}

void draw(cg_uint dt)
{
    background(bg_colour);
    stroke(fg_colour);

    // draw a point at the center of the canvas
    point(width / 2, height / 2, L'X');

    // no loop
    // no_loop();
}