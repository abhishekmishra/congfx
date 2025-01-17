#include "congfx.h"

cg_char *contents = NULL;

cg_uint char_row = 0;
cg_uint char_col = 0;

void setup()
{
    // does nothing
    contents = make_string((width * height) + 1);
    contents[0] = L'\0';
}

void draw(cg_uint dt)
{
    // loop over the contents and print them
    int len = wcslen(contents);
    cg_uint cr = 0, cc = 0;
    for (int i = 0; i < len; i++)
    {
        point(cc, cr, contents[i]);
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
    point(char_col, char_row, c);
    char_col++;
    if (char_col >= width)
    {
        char_col = 0;
        char_row++;
    }

    // append to contents
    wchar_t wc = c;
    contents = wcsncat(contents, &wc, 1);
    contents = wcsncat(contents, L"\0", 1);
}