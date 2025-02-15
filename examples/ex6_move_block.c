#include "congfx.h"

int main(int argc, char *argv[])
{
    cg_uint block_x = 0;
    cg_uint block_y = 0;
    cg_uint block_width = 5;
    cg_uint block_height = 5;
    cg_string block_location_str = cg_make_string(1024);
    block_location_str[0] = L'\0';

    // create the graphics engine
    int err = cg_create_graphics_fullscreen();
    if (err != 0)
    {
        return err;
    }

    block_x = width / 2 - block_width / 2;
    block_y = height / 2 - block_height / 2;

    swprintf(block_location_str, 1024, L"Block Location: (%d, %d)", block_x, block_y);

	while (!cg_should_exit())
	{
		// begin the draw
		cg_begin_draw();

            // clear the canvas
            cg_clear_canvas();

            // draw the block
            cg_rect(block_x, block_y, block_width, block_height);

            // print press escape to exit
            cg_text(L"Press ESC to exit", 0, height - 1);

            // print the block location
            cg_text(block_location_str, 0, height - 2);

            // show usage message
            cg_text(L"Use arrow keys to move the block, 'r' to reset.", 0, 0);

            // end the draw
        cg_end_draw();

        // get the key pressed
        cg_keyboard_input_t inp;
        
        while ((inp = cg_get_key_pressed()).key != CG_KEY_NONE)
        {
            // move the block
            if (inp.key == CG_KEY_LEFT)
            {
                block_x--;
                block_x = cg_clamp(block_x, 0, width - block_width - 1);
            }
            else if (inp.key == CG_KEY_RIGHT)
            {
                block_x++;
                block_x = cg_clamp(block_x, 0, width - block_width - 1);
            }
            else if (inp.key == CG_KEY_UP)
            {
                block_y--;
                block_y = cg_clamp(block_y, 0, height - block_height - 1);
            }
            else if (inp.key == CG_KEY_DOWN)
            {
                block_y++;
                block_y = cg_clamp(block_y, 0, height - block_height - 1);
            }

            // reset if key pressed is 'r'
            if (inp.key == CG_KEY_ALPHANUM && inp.char_value == L'r')
            {
                block_x = width / 2 - block_width / 2;
                block_y = height / 2 - block_height / 2;
            }

            // print the block location
            swprintf(block_location_str, 1024, L"Block Location: (%d, %d)", block_x, block_y);
        }
    }

    // destroy the graphics engine
    cg_destroy_graphics();
}