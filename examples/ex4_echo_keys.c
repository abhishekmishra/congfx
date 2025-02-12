#include "congfx.h"

cg_char *contents = NULL;

cg_uint char_row = 0;
cg_uint char_col = 0;

void setup()
{
    // does nothing
    contents = cg_make_string((width * height) + 1);
    contents[0] = L'\0';
}

void draw(cg_uint dt)
{
    // loop over the contents and print them
    int len = char_row * width + char_col;
    cg_uint cr = 0, cc = 0;
    for (cg_uint i = 0; i < len; i++)
    {
        cg_point(cc, cr, contents[i]);
        cg_point(cc, 10, i+48);
        cc++;
        if (cc >= width)
        {
            cc = 0;
            cr++;
        }
    }
}

void key_pressed(char c)
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