#include <stdlib.h>
#define CONGFX_IMPLEMENTATION
#include "congfx.h"

// get random int between min and max inclusive
int rand_between(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

int main(int argc, char *argv[])
{
    cg_rgb_t bg_colour = {0, 0, 0};

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

        // clear the background
        cg_background(bg_colour);

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                cg_stroke((cg_rgb_t){rand_between(0, 255), rand_between(0, 255), rand_between(0, 255)});
                cg_point(i, j, L'█');
            }
        }

        // reset to white
        cg_stroke((cg_rgb_t){255, 255, 255});

        // print press escape to exit
        cg_text(L"Press ESC to exit", 0, height - 1);

        // end the draw
        cg_end_draw();
    }

    // destroy the graphics engine
    cg_destroy_graphics();
}
