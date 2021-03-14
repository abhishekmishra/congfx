#ifndef __PIECER_H__
#define __PIECER_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

#include <unistd.h>

// typedefs
typedef wchar_t character;
typedef long integer;
typedef unsigned long uinteger;
typedef long double number;

void setup();

void draw(long dt);

void noLoop();

void startLoop();

// system variables
int loop = 1;

// terminal utility functions
void cls();
void home();

// canvas functions
character *canvas_contents = L'\0';
uinteger width;
uinteger height;

#define DEFAULT_FPS 30 // times per second
#define DEFAULT_BACKGROUND_CHAR L' '
uinteger fps = DEFAULT_FPS;
character background_char = DEFAULT_BACKGROUND_CHAR;

void createCanvas();
void background(character c);
void showCanvas();

// drawing functions
void point(uinteger x1, uinteger y1, character c);
void line(uinteger x1, uinteger y1, uinteger x2, uinteger y2);
void rect(uinteger x1, uinteger y1, uinteger width, uinteger height);
void text(character *t, uinteger x, uinteger y);

// internal functions
uinteger _diff_time_micros(struct timespec time1, struct timespec time2)
{
    //wprintf(L"time1 [%ld, %ld], time2[%ld, %ld]\n", time1.tv_sec, time1.tv_nsec, time2.tv_sec, time2.tv_nsec);
    uinteger seconds_delta = (time1.tv_sec - time2.tv_sec);
    integer nanos_delta = (time1.tv_nsec - time2.tv_nsec);
    uinteger delta =  (seconds_delta * 1000000) + (nanos_delta / 1000);
    //wprintf(L"seconds_delta = %lu, nanos_delta = %ld, delta = %lu\n", seconds_delta, nanos_delta, delta);
    return delta;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    fwide(stdout, 1);

    struct timespec start_time, prev_time, current_time, after_draw_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    prev_time = start_time;
    current_time = start_time;

    setup();

    // if there is no canvas created, create a default one
    if (canvas_contents == NULL)
    {
        createCanvas(100, 25);
    }

    uinteger delta_time_ideal = 1000000 / fps; //in microseconds

    while (loop == 1)
    {
        uinteger dt = _diff_time_micros(current_time, prev_time);

        cls();
        home();

        draw(dt);
        showCanvas();

        // how much time spent
        clock_gettime(CLOCK_MONOTONIC, &after_draw_time);
        uinteger dt_done = _diff_time_micros(after_draw_time, current_time);
        // wprintf(L"Delta ideal %lu, Delta done %lu\n", delta_time_ideal, dt_done);
        if (delta_time_ideal > dt_done)
        {
            struct timespec dt_diff, dt_diff_rem;
            dt_diff.tv_sec = 0;
            dt_diff.tv_nsec = (delta_time_ideal - dt_done) * 1000;
            //sleep for the difference
            // wprintf(L"sleep for -> [%ld]nanos\n", dt_diff.tv_nsec);
            nanosleep(&dt_diff, &dt_diff_rem);
        }

        prev_time = current_time;
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        dt_done = _diff_time_micros(current_time, prev_time);
        // wprintf(L"After sleep: Delta ideal %lu, Delta done %lu\n", delta_time_ideal, dt_done);
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

void createCanvas(uinteger w, uinteger h)
{
    if (width != w || height != h)
    {
        width = w;
        height = h;

        if (canvas_contents != NULL)
        {
            free(canvas_contents);
        }

        uinteger canvas_len = ((width + 1) * height) + 1;
        canvas_contents = calloc(sizeof(character), canvas_len);
        if (canvas_contents == NULL)
        {
            printf("Error: unable to allocate canvas.\n");
            exit(-1);
        }

        uinteger idx = 0;
        for (uinteger i = 0; i < height; i++)
        {
            for (uinteger j = 0; j < width; j++)
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

void background(character c)
{
    background_char = c;
    for (uinteger x = 0; x < width; x++)
    {
        for (uinteger y = 0; y < height; y++)
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

void point(uinteger x1, uinteger y1, character c)
{
    if (x1 >= width || y1 >= height || x1 < 0 || y1 < 0)
    {
        return;
    }
    uinteger cvloc = ((width + 1) * y1) + x1;
    canvas_contents[cvloc] = c;
}

void line(uinteger x1, uinteger y1, uinteger x2, uinteger y2)
{
    // swap coordinates if y1 is lower
    if (x1 > x2)
    {
        uinteger tx, ty;
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
        for (uinteger y = y1; y <= y2; y++)
        {
            point(x1, y, L'*');
        }
    }
    else
    {
        double slope = (y2 - y1) / (x2 - x1);
        for (uinteger x = x1; x <= x2; x++)
        {
            uinteger y = y1 + (slope * x);
            point(x, y, L'*');
        }
    }
}

void rect(uinteger x1, uinteger y1, uinteger width, uinteger height)
{
    line(x1, y1, x1 + width, y1);
    line(x1 + width, y1, x1 + width, y1 + height);
    line(x1 + width, y1 + height, x1, y1 + height);
    line(x1, y1, x1, y1 + height);
}

#endif //__PIECER_H__
