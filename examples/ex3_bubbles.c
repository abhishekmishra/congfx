#include "congfx.h"

int main(int argc, char *argv[])
{

    cg_uint total_time = 0;
    cg_uint step_time = 100000;
    cg_uint radius = 1;
    cg_rgb_t bg_colour = {0, 100, 0};

    // create the graphics engine
    int err = cg_create_graphics_fullscreen();
    if (err != 0)
    {
        return err;
    }

	while (!cg_should_exit())
	{
		// begin the draw
		cg_begin_draw();

        cg_background(bg_colour);

        cg_rgb_t col = {0, 0, cg_get_deltatime_micros() % 256};
        cg_stroke(col);

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
        total_time += cg_get_deltatime_micros();

        // end the draw
        cg_end_draw();
    }

    // destroy the graphics engine
    cg_destroy_graphics();
}