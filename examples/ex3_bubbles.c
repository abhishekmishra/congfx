#include "congfx.h"

int main(int argc, char *argv[])
{

    cg_uint total_time = 0;         // total time in millis
    cg_uint step_time = 250;        // time to update the number of lines to draw in millis
    cg_uint radius = 1;             // radius of the circle
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

            cg_rgb_t col = {total_time, 0, 0};
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
            total_time += cg_get_deltatime();

            // set stroke to black
            cg_stroke((cg_rgb_t){0, 0, 0});

            // print press escape to exit
            cg_text(L"Press ESC to exit", 0, height - 1);

        // end the draw
        cg_end_draw();
    }

    // destroy the graphics engine
    cg_destroy_graphics();
}