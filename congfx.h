/*
Copyright 2021-25 Abhishek Mishra <neolateral.in>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __CONGFX_H__
#define __CONGFX_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <math.h>

// TODO: removed for now, check if needed later
#include <unistd.h>

// defaults
#define DEFAULT_FPS 30 // times per second
#define DEFAULT_BACKGROUND_CHAR L' '

#define _CONGFX_CALLOC calloc
#define _CONGFX_FREE free

// typedefs
typedef wchar_t cg_char;
typedef cg_char *cg_string;
typedef long cg_int;
typedef unsigned long cg_uint;
typedef long double cg_number;

cg_uint number_to_uinteger(cg_number x) {
    if(x < 0) {
        return 0;
    } else {
        return (cg_uint)x;
    }
}

typedef cg_number *vec2;

// type utility functions
cg_string make_string(cg_uint length);
void dispose_string(cg_string s);

vec2 make_vec2(cg_number v1, cg_number v2);
vec2 make_vec2_from(vec2 other);
vec2 vec2_add(vec2 v1, vec2 v2);
vec2 vec2_mult_scalar(vec2 vin, cg_number x);
cg_string vec2_to_string(vec2 v);
void dispose_vec2(vec2 v);

int rand_number(int from, int to);

void setup();

void draw(cg_uint dt);

void no_loop();

void loop();

void frame_rate(cg_uint fps);

// system variables
int _loop = 1;
cg_uint _fps = DEFAULT_FPS;
cg_char background_char = DEFAULT_BACKGROUND_CHAR;
cg_number background_colour = 0;
cg_number foreground_colour = 15;

// terminal utility functions
void cls();
void home();
void reset_term();
void set_foreground_colour(cg_int colour);
void set_background_colour(cg_int colour);

// canvas functions
cg_char *canvas_contents = L'\0';
cg_number *canvas_background_colour;
cg_number *canvas_foreground_colour;
cg_uint width;
cg_uint height;

void create_canvas(cg_uint w, cg_uint h);
void background(cg_number c);
void set_colour(cg_number c);
void show_canvas();

// drawing functions
void point(cg_uint x1, cg_uint y1, cg_char c);
void line(cg_uint x1, cg_uint y1, cg_uint x2, cg_uint y2);
void rect(cg_uint x1, cg_uint y1, cg_uint width, cg_uint height);
void text(cg_char *t, cg_uint x, cg_uint y);

// internal functions
cg_uint _diff_time_micros(struct timespec time1, struct timespec time2)
{
    //wprintf(L"time1 [%ld, %ld], time2[%ld, %ld]\n", time1.tv_sec, time1.tv_nsec, time2.tv_sec, time2.tv_nsec);
    cg_uint seconds_delta = (time1.tv_sec - time2.tv_sec);
    cg_int nanos_delta = (time1.tv_nsec - time2.tv_nsec);
    cg_uint delta = (seconds_delta * 1000000) + (nanos_delta / 1000);
    //wprintf(L"seconds_delta = %lu, nanos_delta = %ld, delta = %lu\n", seconds_delta, nanos_delta, delta);
    return delta;
}

int main(int argc, char *argv[])
{
    // init locale for terminal, and wide output
    setlocale(LC_ALL, "");
    fwide(stdout, 1);

    // init random numbers
    srand(time(NULL));

    struct timespec start_time, prev_time, current_time, after_draw_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    prev_time = start_time;
    current_time = start_time;

    setup();

    // if there is no canvas created, create a default one
    if (canvas_contents == NULL)
    {
        create_canvas(100, 25);
    }

    cg_uint delta_time_ideal = 1000000 / _fps; //in microseconds

    while (_loop == 1)
    {
        cg_uint dt = _diff_time_micros(current_time, prev_time);

        // set default background and forground
        reset_term();
        set_foreground_colour(45);
        background(235);
        set_colour(15);

        cls();
        home();

        draw(dt);
        show_canvas();

        // how much time spent
        clock_gettime(CLOCK_MONOTONIC, &after_draw_time);
        cg_uint dt_done = _diff_time_micros(after_draw_time, current_time);
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
        // dt_done = _diff_time_micros(current_time, prev_time);
        // wprintf(L"After sleep: Delta ideal %lu, Delta done %lu\n", delta_time_ideal, dt_done);
    }
}

// type utility functions
cg_string make_string(cg_uint length)
{
    cg_string s = (cg_string)_CONGFX_CALLOC(length + 1, sizeof(cg_char));
    if (s == NULL)
    {
        wprintf(L"FATAL Error: Unable to allocate cg_string.\n");
        exit(-1);
    }
    return s;
}

void dispose_string(cg_string s)
{
    if (s != NULL)
    {
        _CONGFX_FREE(s);
    }
}

vec2 make_vec2(cg_number v0, cg_number v1)
{
    vec2 v = (vec2)_CONGFX_CALLOC(2, sizeof(cg_number));
    if (v == NULL)
    {
        wprintf(L"FATAL Error: Unable to allocate vec2.\n");
        exit(-1);
    }
    v[0] = v0;
    v[1] = v1;
    return v;
}

vec2 make_vec2_from(vec2 other)
{
    vec2 v = make_vec2(other[0], other[1]);
    return v;
}

vec2 vec2_add(vec2 v1, vec2 v2)
{
    vec2 res = make_vec2_from(v1);
    res[0] = res[0] + v2[0];
    res[1] = res[1] + v2[1];
    return res;
}

vec2 vec2_mult_scalar(vec2 vin, cg_number x)
{
    vec2 v = make_vec2_from(vin);
    v[0] = v[0] * x;
    v[1] = v[1] * x;
    return v;
}

cg_string vec2_to_string(vec2 v)
{
    cg_string s = make_string(100);
    swprintf(s, 100, L"%.2Lf, %.2Lf", v[0], v[1]);
    return s;
}

void dispose_vec2(vec2 v)
{
    if (v != NULL)
    {
        _CONGFX_FREE(v);
    }
}

int rand_number(int from, int to)
{
    int num = (rand() % (to - from + 1)) + from;
    return num;
}

void no_loop()
{
    _loop = 0;
}

void loop()
{
    _loop = 1;
}

void frame_rate(cg_uint fps)
{
    if (fps > 0 && fps < 100)
    {
        _fps = fps;
    }
    else
    {
        wprintf(L"FATAL: Invalid fps [%lu], should be in range (0, 100). \n", fps);
        exit(-1);
    }
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

void reset_term()
{
    wprintf(L"\033[0m");
}

void set_foreground_colour(cg_int colour)
{
    if (colour > -1 && colour < 256)
    {
        // putwchar(L'\033');
        // putwchar(L'[');
        // putwchar(L'3');
        // putwchar(L'8');
        // putwchar(L';');
        // putwchar(L'5');
        // putwchar(L';');
        // putwchar(L'1');
        // putwchar(L'5');
        // putwchar(L'm');
        //fputws(L"\033[38;5;8m", stdout);
        // wprintf(L"\033[38;5;%lum", colour);
    }
}

void set_background_colour(cg_int colour)
{
    if (colour > -1 && colour < 256)
    {
        wprintf(L"\033[48;5;%lum", colour);
    }
}

//canvas functions

void create_canvas(cg_uint w, cg_uint h)
{
    if (width != w || height != h)
    {
        width = w;
        height = h;

        if (canvas_contents != NULL)
        {
            _CONGFX_FREE(canvas_contents);
            _CONGFX_FREE(canvas_background_colour);
        }

        cg_uint canvas_len = ((width + 1) * height) + 1;
        canvas_contents = _CONGFX_CALLOC(sizeof(cg_char), canvas_len);
        canvas_background_colour = _CONGFX_CALLOC(sizeof(cg_number), width * height);
        canvas_foreground_colour = _CONGFX_CALLOC(sizeof(cg_number), width * height);
        if (canvas_contents == NULL || canvas_background_colour == NULL || canvas_foreground_colour == NULL)
        {
            printf("Error: unable to allocate canvas.\n");
            exit(-1);
        }

        cg_uint idx = 0;
        for (cg_uint i = 0; i < height; i++)
        {
            for (cg_uint j = 0; j < width; j++)
            {
                canvas_contents[idx] = background_char;
                idx += 1;
            }
            canvas_contents[idx] = L'\n';
            idx += 1;
        }
        canvas_contents[idx] = L'\0';

        for (cg_uint i = 0; i < height; i++)
        {
            for (cg_uint j = 0; j < width; j++)
            {
                canvas_background_colour[(i * width) + j] = background_colour;
                canvas_foreground_colour[(i * width) + j] = foreground_colour;
            }
        }
    }
    else
    {
        // canvas size does not change.
        // TODO: might want to print a warning here;
    }
}

void background(cg_number col)
{
    background_colour = col;
    for (cg_uint x = 0; x < width; x++)
    {
        for (cg_uint y = 0; y < height; y++)
        {
            point(x, y, background_char);
        }
    }
}

void set_colour(cg_number col) {
    foreground_colour = col;
}

void show_canvas()
{
    if (canvas_contents != NULL)
    {
        // wprintf(L"%ls", canvas_contents);
        cg_uint idx = 0;
        for (cg_uint i = 0; i < height; i++)
        {
            for (cg_uint j = 0; j < width; j++)
            {
                // set_foreground_colour(canvas_foreground_colour[(i * width) + j]);
                // set_background_colour(canvas_background_colour[(i * width) + j]);
                putwchar(canvas_contents[idx]);
                idx += 1;
            }
            putwchar(L'\n');
            idx += 1;
        }
    }
}

void point(cg_uint x1, cg_uint y1, cg_char c)
{
    if (x1 >= width || y1 >= height || x1 < 0 || y1 < 0)
    {
        return;
    }
    cg_uint cvloc = ((width + 1) * y1) + x1;
    canvas_contents[cvloc] = c;
    // canvas_background_colour[(y1 * width) + x1] = background_colour;
    // canvas_foreground_colour[(y1 * width) + x1] = foreground_colour;
    // wprintf(L"\033[%lu;%luf", y1, x1);
    // wprintf(L"%c", c);
}

void line(cg_uint x1, cg_uint y1, cg_uint x2, cg_uint y2)
{
    // wprintf(L"line [%lu,%lu] -> [%lu, %lu]", x1, y1, x2, y2);

    // swap coordinates if y1 is lower
    if (x1 > x2)
    {
        cg_uint tx, ty;
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
        for (cg_uint y = y1; y <= y2; y++)
        {
            point(x1, y, L'█');
        }
    }
    else
    {
        double slope = (y2 - y1) / (x2 - x1);
        for (cg_uint x = x1; x <= x2; x++)
        {
            cg_uint y = y1 + (slope * x);
            point(x, y, L'█');
        }
    }
}

void rect(cg_uint x1, cg_uint y1, cg_uint width, cg_uint height)
{
    line(x1, y1, x1 + width, y1);
    line(x1 + width, y1, x1 + width, y1 + height);
    line(x1 + width, y1 + height, x1, y1 + height);
    line(x1, y1, x1, y1 + height);
}

void text(cg_char *t, cg_uint x, cg_uint y)
{
    cg_uint len = wcslen(t);
    if (len > 0)
    {
        for (cg_uint i = 0; i < len; i++)
        {
            point(x + i, y, t[i]);
        }
    }
}

#endif //__CONGFX_H__
