#include "congfx.h"

int main(int argc, char *argv[])
{
    cg_rgb_t fg_colour = {255, 100, 0};
    cg_rgb_t bg_colour = {0, 100, 0};
    
    cg_frame_rate(60);

    // create the graphics engine
    int err = cg_create_graphics_fullscreen();
    if (err != 0)
    {
        return err;
    }

    while(1)
    {
        // begin the draw
        err = cg_begin_draw();
        if (err != 0)
        {
            break;
        }

        cg_background(bg_colour);
        cg_stroke(fg_colour);

        // draw a point at the center of the canvas
        cg_point(width / 2, height / 2, L'X');

        // end the draw
        cg_end_draw();
    }

    // destroy the graphics engine
    cg_destroy_graphics();
}