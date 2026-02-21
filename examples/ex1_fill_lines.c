#define CONGFX_IMPLEMENTATION
#include "congfx.h"

int main(int argc, char *argv[])
{

    cg_uint total_time = 0;
    cg_uint step_time = 50;
    cg_char total_time_string[1024];
    cg_uint lines_to_draw = 0;

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

        // clear the canvas
        cg_clear_canvas();

        // draw the lines
        for (cg_uint i = 0; i < lines_to_draw; i++)
        {
            cg_line(0, i, width, i);
        }

        // draw the total time in the center of the canvas
        snprintf(total_time_string, 1024, "Total time: %5lu", total_time);
        cg_text(total_time_string, (width - strlen(total_time_string)) / 2, 10);
        total_time += cg_get_deltatime();

        // print press escape to exit
        cg_text("Press ESC to exit", 0, height - 1);

        // end the draw
        cg_end_draw();
    }

    // destroy the graphics engine
    cg_destroy_graphics();
}