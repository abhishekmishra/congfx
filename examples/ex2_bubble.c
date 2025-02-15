#include "congfx.h"

int main(int argc, char *argv[])
{

    cg_uint total_time = 0;
    cg_uint step_time = 100000;
    cg_uint radius = 1;
    cg_rgb_t bg_colour = {1, 1, 1};
    cg_rgb_t col = {255, 255, 255};

    cg_frame_rate(60);

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

            cg_clear_canvas();

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
            
            // print press escape to exit
            cg_text(L"Press ESC to exit", 0, height - 1);

        // end the draw
        cg_end_draw();
    }

    // destroy the graphics engine
    cg_destroy_graphics();
}