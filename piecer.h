#ifndef __PIECER_H__
#define __PIECER_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

void setup();

void draw();

void noLoop();

void startLoop();

// system variables
int loop = 1;

// terminal utility functions
void cls();
void home();

// canvas functions
wchar_t *canvas_contents = L'\0';
size_t width;
size_t height;

#define BACKGROUND_CHAR_DEFAULT L' '
wchar_t background_char = BACKGROUND_CHAR_DEFAULT;

void createCanvas();
void background(wchar_t c);
void showCanvas();

// drawing functions
void point(size_t x1, size_t y1, wchar_t c);
void line(size_t x1, size_t y1, size_t x2, size_t y2);
void rect(size_t x1, size_t y1, size_t width, size_t height);

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    fwide(stdout, 1);

    setup();

    // if there is no canvas created, create a default one
    if (canvas_contents == NULL)
    {
        createCanvas(100, 25);
    }

    while (loop == 1)
    {
        cls();
        home();
        draw();
        showCanvas();
    }
}

void noLoop()
{
    loop = 0;
}

void startLoop()
{
    loop = 1;
}

// terminal utility functions
void cls()
{
    wprintf(L"\033[2J");
}

void home()
{
    wprintf(L"\033[H");
}

//canvas functions

void createCanvas(size_t w, size_t h)
{
    if (width != w || height != h)
    {
        width = w;
        height = h;

        if (canvas_contents != NULL)
        {
            free(canvas_contents);
        }

        size_t canvas_len = ((width + 1) * height) + 1;
        canvas_contents = calloc(sizeof(wchar_t), canvas_len);
        if (canvas_contents == NULL)
        {
            printf("Error: unable to allocate canvas.\n");
            exit(-1);
        }

        size_t idx = 0;
        for (size_t i = 0; i < height; i++)
        {
            for (size_t j = 0; j < width; j++)
            {
                canvas_contents[idx] = background_char;
                idx += 1;
            }
            canvas_contents[idx] = L'\n';
            idx += 1;
        }
        canvas_contents[idx] = L'\0';
    }
    else
    {
        // canvas size does not change.
        // TODO: might want to print a warning here;
    }
}

void background(wchar_t c)
{
    background_char = c;
    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            point(x, y, background_char);
        }
    }
}

void showCanvas()
{
    if (canvas_contents != NULL)
    {

        wprintf(L"%ls", canvas_contents);
    }
}

void point(size_t x1, size_t y1, wchar_t c)
{
    if (x1 >= width || y1 >= height || x1 < 0 || y1 < 0)
    {
        return;
    }
    size_t cvloc = ((width + 1) * y1) + x1;
    canvas_contents[cvloc] = c;
}

void line(size_t x1, size_t y1, size_t x2, size_t y2)
{
    // swap coordinates if y1 is lower
    if (x1 > x2)
    {
        size_t tx, ty;
        tx = x2;
        ty = y2;
        x2 = x1;
        y2 = y1;
        x1 = tx;
        y1 = ty;
    }

    if (x1 == x2)
    {
        //slope is infinite
        for (size_t y = y1; y <= y2; y++)
        {
            point(x1, y, L'*');
        }
    }
    else
    {
        double slope = (y2 - y1) / (x2 - x1);
        for (size_t x = x1; x <= x2; x++)
        {
            size_t y = y1 + (slope * x);
            point(x, y, L'*');
        }
    }
}

void rect(size_t x1, size_t y1, size_t width, size_t height)
{
    line(x1, y1, x1 + width, y1);
    line(x1 + width, y1, x1 + width, y1 + height);
    line(x1 + width, y1 + height, x1, y1 + height);
    line(x1, y1, x1, y1 + height);
}

#endif //__PIECER_H__
