#include "congfx.h"

cg_char *contents = NULL;

cg_uint char_row = 0;
cg_uint char_col = 0;

void handle_key_pressed(char c);

int main(int argc, char *argv[])
{
    contents = cg_make_string((width * height) + 1);
    contents[0] = L'\0';

    // create the graphics engine
    int err = cg_create_graphics_fullscreen();
    if (err != 0)
    {
        return err;
    }

    while (1)
    {
        // begin the draw
        err = cg_begin_draw();
        if (err != 0)
        {
            break;
        }

        // loop over the contents and print them
        int len = char_row * width + char_col;
        cg_uint cr = 0, cc = 0;
        for (cg_uint i = 0; i < len; i++)
        {
            cg_point(cc, cr, contents[i]);
            cg_point(cc, 10, i + 48);
            cc++;
            if (cc >= width)
            {
                cc = 0;
                cr++;
            }
        }

        // end the draw
        cg_end_draw();

        // get the key pressed
        char c = cg_get_key_pressed_char();
        if (c != 0)
        {
            handle_key_pressed(c);
        }
    }

    // destroy the graphics engine
    cg_destroy_graphics();
}

void handle_key_pressed(char c)
{
    cg_point(char_col, char_row, c);

    // append to contents
    cg_char wc = (cg_char)c;
    contents[char_row * width + char_col] = wc;

    // set the next position
    char_col++;
    if (char_col >= width)
    {
        char_col = 0;
        char_row++;
    }
}