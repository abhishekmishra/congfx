#define CONGFX_IMPLEMENTATION
#include "congfx.h"

int main(int argc, char *argv[])
{
    cg_rgb_t fg_colour = {0, 0, 0};
    cg_rgb_t bg_colour = {0, 100, 0};

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

        cg_background(bg_colour);
        cg_stroke(fg_colour);

        for (int i = 0; i < width - 1; i++)
        {
            for (int j = 0; j < height - 1; j++)
            {
                cg_stroke((cg_rgb_t){i, j*10, 255});
                cg_point(i, j, L'X');
            }
        }

        // print press escape to exit
        cg_text(L"Press ESC to exit", 0, height - 1);

        // end the draw
        cg_end_draw();
    }

    // destroy the graphics engine
    cg_destroy_graphics();
}
