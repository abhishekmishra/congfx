#include "congfx.h"

cg_rgb_t fg_colour = {255, 100, 0};
cg_rgb_t bg_colour = {0, 100, 0};

void setup()
{
    // cg_background(12);
    // cg_stroke(15);
    cg_frame_rate(60);
}

void draw(cg_uint dt)
{
    cg_background(bg_colour);
    cg_stroke(fg_colour);

    // draw a point at the center of the canvas
    cg_point(width / 2, height / 2, L'X');

    // no loop
    // cg_no_loop();
}

void key_pressed(char c)
{
}