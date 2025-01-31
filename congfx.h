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

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <math.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

// TODO: removed for now, check if needed later
#include <unistd.h>

// defaults
#define _CG_DEFAULT_FPS 30 // times per second
#define _CG_DEFAULT_BACKGROUND_CHAR L' '

#define _CG_CALLOC calloc
#define _CG_FREE free

// typedefs

/**
 * The default character type used in the congfx programs
 * are wide characters (wchar_t).
 */
typedef wchar_t cg_char;

/**
 * The default string type used in the congfx programs
 * are wide strings (wchar_t).
 */
typedef cg_char *cg_string;

/**
 * The default integer type used in the congfx programs
 * are long integers (long).
 */
typedef long cg_int;

/**
 * The default unsigned integer type used in the congfx programs
 * are unsigned long integers (unsigned long).
 */
typedef unsigned long cg_uint;

/**
 * The default number type used in the congfx programs
 * are long double (long double).
 */
typedef long double cg_number;

/**
 * Define a colour type.
 */
typedef struct
{
    cg_uint r;
    cg_uint g;
    cg_uint b;
} cg_rgb_t;

/**
 * Define a cell contents type
 */
typedef struct
{
    cg_char c;
    cg_rgb_t bg;
    cg_rgb_t fg;
} cg_cell_t;

cg_cell_t *make_cell(cg_char c, cg_rgb_t bg, cg_rgb_t fg)
{
    cg_cell_t *cell = (cg_cell_t *)_CG_CALLOC(1, sizeof(cg_cell_t));
    if (cell == NULL)
    {
        wprintf(L"FATAL Error: Unable to allocate cg_cell_t.\n");
        exit(-1);
    }
    cell->c = c;
    cell->bg = bg;
    cell->fg = fg;
    return cell;
}

cg_rgb_t get_cell_bg(cg_cell_t *cell)
{
    if (cell != NULL)
    {
        return cell->bg;
    }
    return (cg_rgb_t){0, 0, 0};
}

cg_rgb_t get_cell_fg(cg_cell_t *cell)
{
    if (cell != NULL)
    {
        return cell->fg;
    }
    return (cg_rgb_t){255, 255, 255};
}

cg_char get_cell_char(cg_cell_t *cell)
{
    if (cell != NULL)
    {
        return cell->c;
    }
    return L' ';
}

void set_cell_bg(cg_cell_t *cell, cg_rgb_t bg)
{
    if (cell != NULL)
    {
        cell->bg = bg;
    }
}

void set_cell_fg(cg_cell_t *cell, cg_rgb_t fg)
{
    if (cell != NULL)
    {
        cell->fg = fg;
    }
}

void set_cell_char(cg_cell_t *cell, cg_char c)
{
    if (cell != NULL)
    {
        cell->c = c;
    }
}

void dispose_cell(cg_cell_t *cell)
{
    if (cell != NULL)
    {
        _CG_FREE(cell);
    }
}

/**
 * Define a canvas type
 */
typedef struct
{
    cg_uint width;
    cg_uint height;
    cg_cell_t *cells;
} cg_canvas_t;

// system variables

int _loop = 1;
cg_uint _fps = _CG_DEFAULT_FPS;
cg_char background_char = _CG_DEFAULT_BACKGROUND_CHAR;
cg_rgb_t default_bg_colour = {0, 0, 0};
cg_rgb_t default_fg_colour = {255, 255, 255};
cg_rgb_t background_colour = {0, 0, 0};
cg_rgb_t stroke_colour = {255, 255, 255};
cg_rgb_t fill_colour = {255, 255, 255};
struct termios orig_termios;
int _cg_term_orig_flags;

// canvas variables for the current and previous canvas
cg_canvas_t *canvas_previous = NULL;
cg_canvas_t *canvas_current = NULL;

// canvas state variables
// cg_char *canvas_contents = NULL;
// cg_rgb_t *canvas_background_colour;
// cg_rgb_t *canvas_foreground_colour;
cg_uint width;
cg_uint height;

cg_canvas_t *make_canvas(cg_uint w, cg_uint h)
{
    cg_canvas_t *canvas = (cg_canvas_t *)_CG_CALLOC(1, sizeof(cg_canvas_t));
    if (canvas == NULL)
    {
        wprintf(L"FATAL Error: Unable to allocate cg_canvas_t.\n");
        exit(-1);
    }
    canvas->width = w;
    canvas->height = h;
    canvas->cells = (cg_cell_t *)_CG_CALLOC(w * h, sizeof(cg_cell_t));
    if (canvas->cells == NULL)
    {
        wprintf(L"FATAL Error: Unable to allocate cg_cell_t array.\n");
        exit(-1);
    }

    // allocate cells
    for (cg_uint i = 0; i < h; i++)
    {
        for (cg_uint j = 0; j < w; j++)
        {
            canvas->cells[(i * w) + j] = *make_cell(_CG_DEFAULT_BACKGROUND_CHAR, default_bg_colour, default_fg_colour);
        }
    }
    return canvas;
}

cg_cell_t *get_cell(cg_canvas_t *canvas, cg_uint x, cg_uint y)
{
    if (canvas == NULL)
    {
        return NULL;
    }
    if (x >= canvas->width || y >= canvas->height)
    {
        return NULL;
    }
    return &(canvas->cells[(y * canvas->width) + x]);
}

void dispose_canvas(cg_canvas_t *canvas)
{
    if (canvas != NULL)
    {
        if (canvas->cells != NULL)
        {
            _CG_FREE(canvas->cells);
        }
        _CG_FREE(canvas);
    }
}

// Error reporting functions

/**
 * Print an error message and exit the program.
 *
 * @param message The error message to print.
 * @param code The error code to exit with.
 */
void cg_err_fatal(cg_string message, cg_uint code);

/**
 * Print an error message and exit the program with a default error code.
 *
 * @param message The error message to print.
 */
void cg_err_fatal_msg(cg_string message);

// type conversion functions

/**
 * Convert a number to an unsigned integer.
 *
 * @param x The number to convert.
 * @return The unsigned integer value of the number.
 */
cg_uint number_to_uinteger(cg_number x);

// Vector type

/**
 * The default vector type used in the congfx programs
 * are arrays of long double (long double *).
 */
typedef cg_number *vec2;

/**
 * Create a new vector of 2 elements.
 *
 * @param v1 The first element of the vector.
 * @param v2 The second element of the vector.
 * @return The new vector.
 */
vec2 make_vec2(cg_number v1, cg_number v2);

/**
 * Create a new vector of 2 elements from another vector.
 *
 * @param other The vector to copy from.
 * @return The new vector.
 */
vec2 make_vec2_from(vec2 other);

/**
 * Add two vectors.
 *
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return The sum of the two vectors.
 */
vec2 vec2_add(vec2 v1, vec2 v2);

/**
 * Multiply a vector by a scalar.
 *
 * @param vin The vector to multiply.
 * @param x The scalar to multiply by.
 * @return The product of the vector and the scalar.
 */
vec2 vec2_mult_scalar(vec2 vin, cg_number x);

/**
 * Convert a vector to a string.
 *
 * @param v The vector to convert.
 * @return The string representation of the vector.
 */
cg_string vec2_to_string(vec2 v);

/**
 * Dispose of a vector.
 *
 * @param v The vector to dispose of.
 */
void dispose_vec2(vec2 v);

// string type utility functions

/**
 * Create a new string of a given length.
 *
 * @param length The length of the string.
 * @return The new string.
 */
cg_string make_string(cg_uint length);

/**
 * Dispose of a string.
 *
 * @param s The string to dispose of.
 */
void dispose_string(cg_string s);

// number utility functions

/**
 * Generate a random integer between two values.
 *
 * @param from The lower bound of the random integer.
 * @param to The upper bound of the random integer.
 * @return The random integer.
 */
int rand_int(int from, int to);

// Life-cycle functions, to be defined by the main program

void setup();

void draw(cg_uint dt);

void key_pressed(char c);

// Graphics System functions
void no_loop();

void loop();

void frame_rate(cg_uint fps);

// Terminal utility functions
void cls();

void home();

// Internal terminal utility functions
/**
 * Enable raw mode for the terminal.
 * see https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
 */
void _cg_term_enable_raw_mode();

/**
 * Disable raw mode for the terminal.
 * see https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
 */
void _cg_term_disable_raw_mode();

/**
 * Reset the terminal to its default state.
 */
void _cg_term_reset();

void _cg_term_set_foreground_colour(cg_rgb_t colour);

void _cg_term_set_background_colour(cg_rgb_t colour);

// Canvas functions
void create_canvas(cg_uint w, cg_uint h);
void background(cg_rgb_t c);
void stroke(cg_rgb_t c);
void fill(cg_rgb_t c);
void set_colour(cg_rgb_t c);
void show_canvas();
void swap_canvas();

// drawing functions
void point(cg_uint x1, cg_uint y1, cg_char c);
void line(cg_uint x1, cg_uint y1, cg_uint x2, cg_uint y2);
void rect(cg_uint x1, cg_uint y1, cg_uint width, cg_uint height);
void text(cg_char *t, cg_uint x, cg_uint y);

// internal functions
cg_uint _diff_time_micros(struct timespec time1, struct timespec time2)
{
    // wprintf(L"time1 [%ld, %ld], time2[%ld, %ld]\n", time1.tv_sec, time1.tv_nsec, time2.tv_sec, time2.tv_nsec);
    cg_uint seconds_delta = (time1.tv_sec - time2.tv_sec);
    cg_int nanos_delta = (time1.tv_nsec - time2.tv_nsec);
    cg_uint delta = (seconds_delta * 1000000) + (nanos_delta / 1000);
    // wprintf(L"seconds_delta = %lu, nanos_delta = %ld, delta = %lu\n", seconds_delta, nanos_delta, delta);
    return delta;
}

int main(int argc, char *argv[])
{
    cg_char read_buf[20];

    // enable raw mode for terminal
    _cg_term_enable_raw_mode();

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
    if (canvas_current == NULL)
    {
        create_canvas(100, 25);
    }

    cg_uint delta_time_ideal = 1000000 / _fps; // in microseconds

    while (_loop == 1)
    {
        cg_uint dt = _diff_time_micros(current_time, prev_time);

        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
        {
            cg_err_fatal_msg(L"read");
        }
        // if (iscntrl(c))
        // {
        //     wprintf(L"%d\r\n", c);
        // }
        // else
        // {
        //     wprintf(L"%d ('%c')\r\n", c, c);
        // }
        if (c == 'q')
        {
            break;
        }
        key_pressed(c);

        // no_loop();

        // set default background and forground
        _cg_term_reset();
        _cg_term_set_foreground_colour(default_fg_colour);
        background(default_bg_colour);
        set_colour(default_fg_colour);

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
            // sleep for the difference
            //  wprintf(L"sleep for -> [%ld]nanos\n", dt_diff.tv_nsec);
            nanosleep(&dt_diff, &dt_diff_rem);
        }

        prev_time = current_time;
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        // dt_done = _diff_time_micros(current_time, prev_time);
        // wprintf(L"After sleep: Delta ideal %lu, Delta done %lu\n", delta_time_ideal, dt_done);

        // swap canvas
        swap_canvas();
    }
}

void cg_err_fatal(cg_string message, cg_uint code)
{
    wprintf(L"FATAL: %ls\n", message);
    exit(code);
}

void cg_err_fatal_msg(cg_string message)
{
    cg_err_fatal(message, -1);
}

cg_uint number_to_uinteger(cg_number x)
{
    if (x < 0)
    {
        return 0;
    }
    else
    {
        return (cg_uint)x;
    }
}

// type utility functions
cg_string make_string(cg_uint length)
{
    cg_string s = (cg_string)_CG_CALLOC(length + 1, sizeof(cg_char));
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
        _CG_FREE(s);
    }
}

vec2 make_vec2(cg_number v0, cg_number v1)
{
    vec2 v = (vec2)_CG_CALLOC(2, sizeof(cg_number));
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
        _CG_FREE(v);
    }
}

int rand_int(int from, int to)
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

void _cg_term_enable_raw_mode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    {
        cg_err_fatal_msg(L"tcgetattr");
    }

    atexit(_cg_term_disable_raw_mode);
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
        cg_err_fatal_msg(L"tcsetattr");
    }

    // Get the current flags
    if ((_cg_term_orig_flags = fcntl(STDIN_FILENO, F_GETFL, 0)) == -1)
    {
        cg_err_fatal_msg(L"fcntl error getting flags");
    }

    // Set the flags to be non-blocking
    if ((fcntl(STDIN_FILENO, F_SETFL, _cg_term_orig_flags | O_NONBLOCK) == -1))
    {
        cg_err_fatal_msg(L"fcntl error setting flags");
    }
}

void _cg_term_disable_raw_mode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    {
        cg_err_fatal_msg(L"tcsetattr");
    }

    // Reset the flags
    if (fcntl(STDIN_FILENO, F_SETFL, _cg_term_orig_flags) == -1)
    {
        cg_err_fatal_msg(L"fcntl error resetting flags");
    }
}

void _cg_term_reset()
{
    wprintf(L"\033[0m");
}

void _cg_term_set_foreground_colour(cg_rgb_t colour)
{
    wprintf(L"\033[38;2;%lu;%lu;%lum", colour.r, colour.g, colour.b);
}

void _cg_term_set_background_colour(cg_rgb_t colour)
{
    wprintf(L"\033[48;2;%lu;%lu;%lum", colour.r, colour.g, colour.b);
}

// canvas functions

void create_canvas(cg_uint w, cg_uint h)
{
    if (canvas_current != NULL)
    {
        dispose_canvas(canvas_current);
    }
    canvas_current = make_canvas(w, h);
    if (canvas_previous != NULL)
    {
        dispose_canvas(canvas_previous);
    }
    canvas_previous = make_canvas(w, h);

    width = w;
    height = h;
}

void swap_canvas()
{
    cg_canvas_t *temp = canvas_current;
    canvas_current = canvas_previous;
    canvas_previous = temp;
}

void background(cg_rgb_t col)
{
    background_colour = col;
    for (cg_uint x = 0; x < canvas_current->width; x++)
    {
        for (cg_uint y = 0; y < canvas_current->height; y++)
        {
            point(x, y, background_char);
        }
    }
}

void stroke(cg_rgb_t col)
{
    stroke_colour = col;
}

void fill(cg_rgb_t col)
{
    fill_colour = col;
}

void set_colour(cg_rgb_t col)
{
    stroke(col);
    fill(col);
}

void show_canvas()
{
    if (canvas_current != NULL)
    {
        cg_rgb_t current_fg = {255, 255, 255};
        cg_rgb_t current_bg = {0, 0, 0};
        // wprintf(L"%ls", canvas_contents);
        cg_uint idx = 0;
        for (cg_uint i = 0; i < canvas_current->height; i++)
        {
            for (cg_uint j = 0; j < canvas_current->width; j++)
            {
                cg_cell_t *cell = get_cell(canvas_current, j, i);
                cg_rgb_t cell_fg = get_cell_fg(cell);
                cg_rgb_t cell_bg = get_cell_bg(cell);

                if (cell_fg.r != current_fg.r || cell_fg.g != current_fg.g || cell_fg.b != current_fg.b)
                {
                    _cg_term_set_foreground_colour(cell_fg);
                    current_fg = cell_fg;
                }
                if (cell_bg.r != current_bg.r || cell_bg.g != current_bg.g || cell_bg.b != current_bg.b)
                {
                    _cg_term_set_background_colour(cell_bg);
                    current_bg = cell_bg;
                }
                // _cg_term_set_foreground_colour(canvas_foreground_colour[(i * width) + j]);
                // _cg_term_set_background_colour(canvas_background_colour[(i * width) + j]);
                cg_char c = get_cell_char(cell);
                putwchar(c);
                // idx += 1;
            }

            putwchar(L'\r');
            putwchar(L'\n');

            // idx += 1;
        }
    }
}

void point(cg_uint x1, cg_uint y1, cg_char c)
{
    if (x1 >= canvas_current->width || y1 >= canvas_current->height || x1 < 0 || y1 < 0)
    {
        return;
    }
    cg_cell_t *cell = get_cell(canvas_current, x1, y1);
    set_cell_char(cell, c);
    set_cell_bg(cell, background_colour);
    set_cell_fg(cell, stroke_colour);
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
        // slope is infinite
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
