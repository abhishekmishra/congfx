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

/**
 * @file congfx.h
 * @brief A simple graphics library for the terminal.
 *
 * This library provides a simple graphics library for the terminal.
 **/

/*
********************************************************************************
***************************** VERSION HISTORY **********************************
********************************************************************************

Date       | Version | Comments
--------------------------------------------------------------------------------
2025/02/01 | 0.1a    | An initial internal release trying out various
           |         | experiments. At this time, not sure if this will ever be
           |         | good enough to release.

********************************************************************************
********************************************************************************
*/

/*
********************************************************************************
********************************** README **************************************
********************************************************************************
ConGfx - A simple graphics exploration library for the terminal.

This graphics exploration library is inspired by p5.js, love2d, and the kilo
text editor and corresponding tutorial to build an editor for the terminal from
scratch.

I've noticed that terminals over time have become quite capable, and looking at
the cool things being built for the terminal I wondered how far I could go in
building p5.js or love2d like game/graphics loop. Can we draw stuff on the
terminal and refresh the drawing fast enough to create the illusion of moving
pictures.

********************************************************************************
********************************************************************************
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
#include <sys/ioctl.h>

// TODO: removed for now, check if needed later
#include <unistd.h>

// defaults
#define _CG_DEFAULT_FPS 30 // times per second
#define _CG_DEFAULT_BACKGROUND_CHAR L' '

#define _CG_CALLOC calloc
#define _CG_REALLOC realloc
#define _CG_FREE free

#define _CG_TERM_COMMAND_BUFFER_START_SIZE 1024
#define _CG_TERM_COMMAND_BUFFER_FLUSH_LIMIT 1023

/*--------- BEGIN TYPE DEFINITIONS -----------*/

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

/**
 * Define a canvas type
 */
typedef struct
{
    cg_uint width;
    cg_uint height;
    cg_cell_t *cells;
} cg_canvas_t;

// Vector type

/**
 * The default vector type used in the congfx programs
 * are arrays of long double (long double *).
 */
typedef cg_number *vec2;

/**
 * A structure to hold a command buffer for the terminal.
 */
typedef struct
{
    cg_char *buffer;
    size_t length;
    size_t size;
} _cg_term_command_buffer_t;

/*--------- END TYPE DEFINITIONS -----------*/

/*--------- BEGIN PUBLIC FUNCTION PROTOTYPES -----------*/

/*+++++++++ BEGIN Cell TYPE FUNCTIONS +++++++++*/

/**
 * Compare two colours.
 *
 * @param colour1 The first colour to compare.
 * @param colour2 The second colour to compare.
 * @return 0 if the colours are equal, -1 otherwise.
 */
int cg_compare_colour(cg_rgb_t colour1, cg_rgb_t colour2);

/**
 * Create a new cell with the given character and colours.
 *
 * @param c The character to display in the cell.
 * @param bg The background colour of the cell.
 * @param fg The foreground colour of the cell.
 * @return The new cell.
 */
cg_cell_t *cg_make_cell(cg_char c, cg_rgb_t bg, cg_rgb_t fg);

/**
 * Get the background colour of a cell.
 *
 * @param cell The cell to get the background colour of.
 * @return The background colour of the cell.
 */
cg_rgb_t cg_get_cell_bg(cg_cell_t *cell);

/**
 * Get the foreground colour of a cell.
 *
 * @param cell The cell to get the foreground colour of.
 * @return The foreground colour of the cell.
 */
cg_rgb_t cg_get_cell_fg(cg_cell_t *cell);

/**
 * Get the character of a cell.
 *
 * @param cell The cell to get the character of.
 * @return The character of the cell.
 */
cg_char cg_get_cell_char(cg_cell_t *cell);

/**
 * Set the background colour of a cell.
 *
 * @param cell The cell to set the background colour of.
 * @param bg The background colour to set.
 */
void cg_set_cell_bg(cg_cell_t *cell, cg_rgb_t bg);

/**
 * Set the foreground colour of a cell.
 *
 * @param cell The cell to set the foreground colour of.
 * @param fg The foreground colour to set.
 */
void cg_set_cell_fg(cg_cell_t *cell, cg_rgb_t fg);

/**
 * Set the character of a cell.
 *
 * @param cell The cell to set the character of.
 * @param c The character to set.
 */
void cg_set_cell_char(cg_cell_t *cell, cg_char c);

/**
 * Dispose of a cell.
 *
 * @param cell The cell to dispose of.
 */
void cg_dispose_cell(cg_cell_t *cell);

/**
 * Compare two cells.
 *
 * @param cell1 The first cell to compare.
 * @param cell2 The second cell to compare.
 * @return 0 if the cells are equal, -1 otherwise.
 */
int cg_compare_cells(cg_cell_t *cell1, cg_cell_t *cell2);

/*+++++++++ END Cell TYPE FUNCTIONS +++++++++*/

/*+++++++++ BEGIN Canvas TYPE FUNCTIONS +++++++++*/

/**
 * Create a new canvas with the given width and height.
 *
 * @param w The width of the canvas.
 * @param h The height of the canvas.
 * @return The new canvas.
 */
cg_canvas_t *cg_make_canvas(cg_uint w, cg_uint h);

/**
 * Get the cell at the given coordinates in the canvas.
 *
 * @param canvas The canvas to get the cell from.
 * @param x The x-coordinate of the cell.
 * @param y The y-coordinate of the cell.
 * @return The cell at the given coordinates.
 */
cg_cell_t *cg_get_cell(cg_canvas_t *canvas, cg_uint x, cg_uint y);

/**
 * Dispose of a canvas.
 *
 * @param canvas The canvas to dispose of.
 */
void cg_dispose_canvas(cg_canvas_t *canvas);

/*+++++++++ END Canvas TYPE FUNCTIONS +++++++++*/

/*+++++++++ BEGIN Vec2 TYPE FUNCTIONS +++++++++*/

/**
 * Create a new vector of 2 elements.
 *
 * @param v1 The first element of the vector.
 * @param v2 The second element of the vector.
 * @return The new vector.
 */
vec2 cg_make_vec2(cg_number v1, cg_number v2);

/**
 * Create a new vector of 2 elements from another vector.
 *
 * @param other The vector to copy from.
 * @return The new vector.
 */
vec2 cg_make_vec2_from(vec2 other);

/**
 * Add two vectors.
 *
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @return The sum of the two vectors.
 */
vec2 cg_vec2_add(vec2 v1, vec2 v2);

/**
 * Multiply a vector by a scalar.
 *
 * @param vin The vector to multiply.
 * @param x The scalar to multiply by.
 * @return The product of the vector and the scalar.
 */
vec2 cg_vec2_mult_scalar(vec2 vin, cg_number x);

/**
 * Convert a vector to a string.
 *
 * @param v The vector to convert.
 * @return The string representation of the vector.
 */
cg_string cg_vec2_to_string(vec2 v);

/**
 * Dispose of a vector.
 *
 * @param v The vector to dispose of.
 */
void cg_dispose_vec2(vec2 v);

/*+++++++++ END Vec2 TYPE FUNCTIONS +++++++++*/

/*+++++++++ BEGIN String TYPE FUNCTIONS +++++++++*/

/**
 * Create a new string of a given length.
 *
 * @param length The length of the string.
 * @return The new string.
 */
cg_string cg_make_string(cg_uint length);

/**
 * Dispose of a string.
 *
 * @param s The string to dispose of.
 */
void cg_dispose_string(cg_string s);

/*+++++++++ END String TYPE FUNCTIONS +++++++++*/

/*+++++++++ BEGIN Number TYPE FUNCTIONS +++++++++*/

/**
 * Convert a number to an unsigned integer.
 *
 * @param x The number to convert.
 * @return The unsigned integer value of the number.
 */
cg_uint cg_number_to_uinteger(cg_number x);

// number utility functions

/**
 * Generate a random integer between two values.
 *
 * @param from The lower bound of the random integer.
 * @param to The upper bound of the random integer.
 * @return The random integer.
 */
int cg_rand_int(int from, int to);

/*+++++++++ END Number TYPE FUNCTIONS +++++++++*/

/*+++++++++ BEGIN ERROR REPORTING UTILITY FUNCTIONS +++++++++*/

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

/*+++++++++ END ERROR REPORTING UTILITY FUNCTIONS +++++++++*/

/*+++++++++ BEGIN LIFECYCLE FUNCTIONS +++++++++*/

/**
 * Create a graphics system with the given width and height.
 * 
 * @param w width
 * @param h height
 * @return int error code
 */
int cg_create_graphics(cg_uint w, cg_uint h);

/**
 * Create a graphics system in fullscreen mode.
 * 
 * @return int error code
 */
int cg_create_graphics_fullscreen();

/**
 * Begin the drawing process.
 * Call before running any draw commands, inside the draw loop.
 * 
 * @return int error code
 */
int cg_begin_draw();

/**
 * End the drawing process.
 * Call after running all draw commands, inside the draw loop.
 * 
 * @return int error code
 */
void cg_end_draw();

/**
 * Destroy the graphics system.
 */
void cg_destroy_graphics();

/**
 * Get the delta time in microseconds.
 * 
 * @return cg_uint delta time in microseconds
 */
cg_uint cg_get_deltatime_micros();

/**
 * Get the delta time in milliseconds.
 * 
 * @return cg_uint delta time in milliseconds
 */
cg_uint cg_get_deltatime_millis();

/**
 * The default delta time function returns
 * the delta time in milliseconds.
 */
#define cg_get_deltatime cg_get_deltatime_millis

/*+++++++++ END LIFECYCLE FUNCTIONS +++++++++*/

/*+++++++++ BEGIN Graphics FUNCTIONS +++++++++*/

// Graphics System functions
void cg_no_loop();

void cg_loop();

void cg_frame_rate(cg_uint fps);

// Terminal utility functions
void cg_cls();

void cg_home();

/*========= BEGIN Graphics Canvas FUNCTIONS =========*/
// Canvas functions
void cg_create_canvas(cg_uint w, cg_uint h);
void cg_background(cg_rgb_t c);
void cg_stroke(cg_rgb_t c);
void cg_fill(cg_rgb_t c);
void cg_set_colour(cg_rgb_t c);
void cg_show_canvas();
void cg_swap_canvas();
void cg_clear_canvas();
/*========= END Graphics Canvas FUNCTIONS =========*/

/*========= BEGIN Graphics Drawing FUNCTIONS =========*/

// drawing functions
void cg_point(cg_uint x1, cg_uint y1, cg_char c);
void cg_line(cg_uint x1, cg_uint y1, cg_uint x2, cg_uint y2);
void cg_rect(cg_uint x1, cg_uint y1, cg_uint width, cg_uint height);
void cg_text(cg_char *t, cg_uint x, cg_uint y);

/*========= END Graphics Drawing FUNCTIONS =========*/

/*+++++++++ END Graphics FUNCTIONS +++++++++*/

/*+++++++++ BEGIN Input FUNCTIONS +++++++++*/

char cg_get_key_pressed_char();
int cg_is_key_pressed(char c);

/*+++++++++ END Input FUNCTIONS +++++++++*/

/*--------- END PUBLIC FUNCTION PROTOTYPES -----------*/

/*--------- BEGIN PUBLIC VARIABLES -----------*/

cg_uint width;
cg_uint height;

/*--------- END PUBLIC VARIABLES -----------*/

/*--------- BEGIN PRIVATE VARIABLES -----------*/

// system variables

typedef struct {
    cg_char read_buf[20];
    struct timespec start_time, prev_time, current_time, after_draw_time;
    cg_uint delta_time_ideal;
    cg_uint dt;
    char key_pressed;
} _cg_graphics_context_t;

_cg_graphics_context_t *_cg_gfx_context = NULL;

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

// command buffer for the terminal
_cg_term_command_buffer_t *_cg_buffer = NULL;

/*--------- END PRIVATE VARIABLES -----------*/

/*--------- BEGIN INTERNAL FUNCTION PROTOTYPES -----------*/

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
 * Create a new command buffer for the terminal.
 *
 * @param buffer The command buffer to create.
 * @return 0 if successful, -1 otherwise.
 */
int _cg_term_create_command_buffer(_cg_term_command_buffer_t **buffer);

/**
 * Expand the command buffer for the terminal.
 *
 * @param buffer The command buffer to expand.
 * @param more_required The additional space required.
 * @return 0 if successful, -1 otherwise.
 */
int _cg_term_expand_command_buffer(_cg_term_command_buffer_t *buffer, size_t more_required);

/**
 * Dispose of a command buffer for the terminal.
 *
 * @param buffer The command buffer to dispose of.
 */
void _cg_term_dispose_command_buffer(_cg_term_command_buffer_t *buffer);

/**
 * Add a command to the command buffer for the terminal.
 *
 * @param buffer The command buffer to add to.
 * @param command The command to add.
 * @param length The length of the command. (if 0, then the length is calculated)
 * @return 0 if successful, -1 otherwise.
 */
int _cg_term_buffer_command(_cg_term_command_buffer_t *buffer,
                            cg_string command, size_t length);

/**
 * Flush the command buffer for the terminal.
 * The commands are written to the terminal, and the buffer is reset.
 *
 * @param buffer The command buffer to flush.
 * @return 0 if successful, -1 otherwise.
 */
int _cg_term_flush_command_buffer(_cg_term_command_buffer_t *buffer);

/**
 * Reset the terminal to its default state.
 */
void _cg_term_reset();

void _cg_term_set_foreground_colour(cg_rgb_t colour);

void _cg_term_set_background_colour(cg_rgb_t colour);

void _cg_term_move_to(cg_uint x, cg_uint y);

void _cg_term_write_char(cg_char c);

void _cg_hide_cursor();

void _cg_show_cursor();

/**
 * Get the cursor position in the terminal.
 * see https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#window-size-the-hard-way
 * @param rows The row position of the cursor.
 * @param cols The column position of the cursor.
 * @return 0 if successful, -1 otherwise.
 */
int _cg_get_cursor_position(int *rows, int *cols);

/**
 * Get the window size of the terminal.
 * see https://viewsourcecode.org/snaptoken/kilo/03.rawInputAndOutput.html#window-size-the-hard-way
 * @param rows The number of rows in the terminal.
 * @param cols The number of columns in the terminal.
 * @return 0 if successful, -1 otherwise.
 */
int _cg_get_window_size(int *rows, int *cols);

/**
 * Read a key press from the terminal.
 */
void _cg_read_key();

// internal functions

/**
 * Calculate the difference in time between two timespec structures.
 *
 * @param time1 The first time.
 * @param time2 The second time.
 * @return The difference in time in microseconds.
 */
cg_uint _diff_time_micros(struct timespec time1, struct timespec time2);

/*--------- END INTERNAL FUNCTION PROTOTYPES -----------*/

int cg_compare_colour(cg_rgb_t colour1, cg_rgb_t colour2)
{
    if (colour1.r != colour2.r
        || colour1.g != colour2.g
        || colour1.b != colour2.b)
    {
        return -1;
    }
    return 0;
}

cg_cell_t *cg_make_cell(cg_char c, cg_rgb_t bg, cg_rgb_t fg)
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

cg_rgb_t cg_get_cell_bg(cg_cell_t *cell)
{
    if (cell != NULL)
    {
        return cell->bg;
    }
    return (cg_rgb_t){0, 0, 0};
}

cg_rgb_t cg_get_cell_fg(cg_cell_t *cell)
{
    if (cell != NULL)
    {
        return cell->fg;
    }
    return (cg_rgb_t){255, 255, 255};
}

cg_char cg_get_cell_char(cg_cell_t *cell)
{
    if (cell != NULL)
    {
        return cell->c;
    }
    return L' ';
}

void cg_set_cell_bg(cg_cell_t *cell, cg_rgb_t bg)
{
    if (cell != NULL)
    {
        cell->bg = bg;
    }
}

void cg_set_cell_fg(cg_cell_t *cell, cg_rgb_t fg)
{
    if (cell != NULL)
    {
        cell->fg = fg;
    }
}

void cg_set_cell_char(cg_cell_t *cell, cg_char c)
{
    if (cell != NULL)
    {
        cell->c = c;
    }
}

void cg_dispose_cell(cg_cell_t *cell)
{
    if (cell != NULL)
    {
        _CG_FREE(cell);
    }
}

int cg_compare_cells(cg_cell_t *cell1, cg_cell_t *cell2)
{
    if (cell1 == NULL && cell2 == NULL)
    {
        return 0;
    }
    if (cell1 == NULL || cell2 == NULL)
    {
        return -1;
    }
    if (cell1->c != cell2->c)
    {
        return -1;
    }
    if (cell1->bg.r != cell2->bg.r || cell1->bg.g != cell2->bg.g || cell1->bg.b != cell2->bg.b)
    {
        return -1;
    }
    if (cell1->fg.r != cell2->fg.r || cell1->fg.g != cell2->fg.g || cell1->fg.b != cell2->fg.b)
    {
        return -1;
    }
    return 0;
}

cg_canvas_t *cg_make_canvas(cg_uint w, cg_uint h)
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
            canvas->cells[(i * w) + j] = *cg_make_cell(_CG_DEFAULT_BACKGROUND_CHAR, default_bg_colour, default_fg_colour);
        }
    }
    return canvas;
}

cg_cell_t *cg_get_cell(cg_canvas_t *canvas, cg_uint x, cg_uint y)
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

void cg_dispose_canvas(cg_canvas_t *canvas)
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
cg_uint _diff_time_micros(struct timespec time1, struct timespec time2)
{
    // wprintf(L"time1 [%ld, %ld], time2[%ld, %ld]\n", time1.tv_sec, time1.tv_nsec, time2.tv_sec, time2.tv_nsec);
    cg_uint seconds_delta = (time1.tv_sec - time2.tv_sec);
    cg_int nanos_delta = (time1.tv_nsec - time2.tv_nsec);
    cg_uint delta = (seconds_delta * 1000000) + (nanos_delta / 1000);
    // wprintf(L"seconds_delta = %lu, nanos_delta = %ld, delta = %lu\n", seconds_delta, nanos_delta, delta);
    return delta;
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

cg_uint cg_number_to_uinteger(cg_number x)
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
cg_string cg_make_string(cg_uint length)
{
    cg_string s = (cg_string)_CG_CALLOC(length + 1, sizeof(cg_char));
    if (s == NULL)
    {
        wprintf(L"FATAL Error: Unable to allocate cg_string.\n");
        exit(-1);
    }
    return s;
}

void cg_dispose_string(cg_string s)
{
    if (s != NULL)
    {
        _CG_FREE(s);
    }
}

vec2 cg_make_vec2(cg_number v0, cg_number v1)
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

vec2 cg_make_vec2_from(vec2 other)
{
    vec2 v = cg_make_vec2(other[0], other[1]);
    return v;
}

vec2 cg_vec2_add(vec2 v1, vec2 v2)
{
    vec2 res = cg_make_vec2_from(v1);
    res[0] = res[0] + v2[0];
    res[1] = res[1] + v2[1];
    return res;
}

vec2 cg_vec2_mult_scalar(vec2 vin, cg_number x)
{
    vec2 v = cg_make_vec2_from(vin);
    v[0] = v[0] * x;
    v[1] = v[1] * x;
    return v;
}

cg_string cg_vec2_to_string(vec2 v)
{
    cg_string s = cg_make_string(100);
    swprintf(s, 100, L"%.2Lf, %.2Lf", v[0], v[1]);
    return s;
}

void cg_dispose_vec2(vec2 v)
{
    if (v != NULL)
    {
        _CG_FREE(v);
    }
}

int cg_rand_int(int from, int to)
{
    int num = (rand() % (to - from + 1)) + from;
    return num;
}

void cg_no_loop()
{
    _loop = 0;
}

void cg_loop()
{
    _loop = 1;
}

void cg_frame_rate(cg_uint fps)
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
void cg_cls()
{
    wprintf(L"\033[2J");
}

void cg_home()
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

int _cg_term_create_command_buffer(_cg_term_command_buffer_t **buffer)
{
    *buffer = (_cg_term_command_buffer_t *)_CG_CALLOC(1, sizeof(_cg_term_command_buffer_t));
    if (*buffer == NULL)
    {
        return -1;
    }
    (*buffer)->buffer = (cg_char *)_CG_CALLOC(_CG_TERM_COMMAND_BUFFER_START_SIZE, sizeof(cg_char));
    if ((*buffer)->buffer == NULL)
    {
        return -1;
    }
    (*buffer)->length = 0;
    // Mark the first character as the null terminator
    (*buffer)->buffer[0] = L'\0';
    (*buffer)->size = _CG_TERM_COMMAND_BUFFER_START_SIZE;
    return 0;
}

int _cg_term_expand_command_buffer(_cg_term_command_buffer_t *buffer, size_t more_required)
{
    // use realloc to expand the buffer
    size_t new_size = buffer->size + more_required;
    cg_char *new_buffer = (cg_char *)_CG_REALLOC(buffer->buffer, new_size * sizeof(cg_char));
    if (new_buffer == NULL)
    {
        return -1;
    }
    buffer->buffer = new_buffer;
    buffer->size = new_size;
    return 0;
}

void _cg_term_dispose_command_buffer(_cg_term_command_buffer_t *buffer)
{
    if (buffer != NULL)
    {
        if (buffer->buffer != NULL)
        {
            _CG_FREE(buffer->buffer);
        }
        _CG_FREE(buffer);
    }
}

int _cg_term_buffer_command(_cg_term_command_buffer_t *buffer, cg_string command, size_t length)
{
    size_t command_length = length;
    if (command_length == 0)
    {
        command_length = wcslen(command);
    }

    size_t new_size = buffer->length + command_length + 1;
    if (new_size > buffer->size)
    {
        if (_cg_term_expand_command_buffer(buffer, command_length) == -1)
        {
            return -1;
        }
    }
    wcsncat(buffer->buffer, command, command_length);
    buffer->length = wcslen(buffer->buffer);

    if (buffer->length >= _CG_TERM_COMMAND_BUFFER_FLUSH_LIMIT)
    {
        return _cg_term_flush_command_buffer(buffer);
    }

    return 0;
}

int _cg_term_flush_command_buffer(_cg_term_command_buffer_t *buffer)
{
    fputws(buffer->buffer, stdout);
    fflush(stdout);
    buffer->length = 0;
    buffer->buffer[0] = L'\0';
    return 0;
}

void _cg_term_reset()
{
    // the [0m code resets all terminal attributes
    _cg_term_buffer_command(_cg_buffer, L"\033[0m", 4);
}

void _cg_term_set_foreground_colour(cg_rgb_t colour)
{
    cg_char buffer[25];
    swprintf(buffer, 25, L"\033[38;2;%lu;%lu;%lum", colour.r, colour.g, colour.b);
    _cg_term_buffer_command(_cg_buffer, buffer, 0);// 22);
}

void _cg_term_set_background_colour(cg_rgb_t colour)
{
    cg_char buffer[25];
    swprintf(buffer, 25, L"\033[48;2;%lu;%lu;%lum", colour.r, colour.g, colour.b);
    _cg_term_buffer_command(_cg_buffer, buffer, 0);// 22);
}

void _cg_term_move_to(cg_uint x, cg_uint y)
{
    cg_char buffer[20];
    swprintf(buffer, 20, L"\033[%lu;%luf", y+1, x+1);
    _cg_term_buffer_command(_cg_buffer, buffer, 0);
}

void _cg_term_write_char(cg_char c)
{
    _cg_term_buffer_command(_cg_buffer, &c, 1);
}

void _cg_hide_cursor()
{
    _cg_term_buffer_command(_cg_buffer, L"\033[?25l", 6);
}

void _cg_show_cursor()
{
    _cg_term_buffer_command(_cg_buffer, L"\033[?25h", 6);
}

int _cg_get_cursor_position(int *rows, int *cols)
{
    char buf[32];
    unsigned int i = 0;
    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
        return -1;
    while (i < sizeof(buf) - 1)
    {
        if (read(STDIN_FILENO, &buf[i], 1) != 1)
            break;
        if (buf[i] == 'R')
            break;
        i++;
    }
    buf[i] = '\0';
    if (buf[0] != '\x1b' || buf[1] != '[')
        return -1;
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
        return -1;
    return 0;
}

int _cg_get_window_size(int *rows, int *cols)
{
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
            return -1;
        return _cg_get_cursor_position(rows, cols);
    }
    else
    {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

void _cg_read_key()
{
    // read one character from the terminal
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
    if (c != '\0')
    {
        _cg_gfx_context->key_pressed = c;
    }
    else
    {
        _cg_gfx_context->key_pressed = 0;
    }
}

// canvas functions

void cg_create_canvas(cg_uint w, cg_uint h)
{
    if (canvas_current != NULL)
    {
        cg_dispose_canvas(canvas_current);
    }
    canvas_current = cg_make_canvas(w, h);
    if (canvas_previous != NULL)
    {
        cg_dispose_canvas(canvas_previous);
    }
    canvas_previous = cg_make_canvas(w, h);

    width = w;
    height = h;
}

void cg_swap_canvas()
{
    cg_canvas_t *temp = canvas_current;
    canvas_current = canvas_previous;
    canvas_previous = temp;
}

void cg_background(cg_rgb_t col)
{
    background_colour = col;
    for (cg_uint x = 0; x < canvas_current->width; x++)
    {
        for (cg_uint y = 0; y < canvas_current->height; y++)
        {
            cg_point(x, y, background_char);
        }
    }
}

void cg_stroke(cg_rgb_t col)
{
    stroke_colour = col;
}

void cg_fill(cg_rgb_t col)
{
    fill_colour = col;
}

void cg_set_colour(cg_rgb_t col)
{
    cg_stroke(col);
    cg_fill(col);
}

void cg_show_canvas()
{
    _cg_hide_cursor();

    cg_rgb_t current_bg = {300, 300, 300};
    cg_rgb_t current_fg = {300, 300, 300};

    if (canvas_current != NULL)
    {
        for (cg_uint i = 0; i < canvas_current->height; i++)
        {
            for (cg_uint j = 0; j < canvas_current->width; j++)
            {
                cg_cell_t *current_cell = cg_get_cell(canvas_current, j, i);
                // cg_cell_t *previous_cell = cg_get_cell(canvas_previous, j, i);
                // if (cg_compare_cells(current_cell, previous_cell) == 0)
                // {
                //     continue;
                // }

                // wprintf(L"Cells not equal at [%lu, %lu]\n", j, i);

                // cg_no_loop();

                // get the cell colours
                cg_rgb_t cell_fg = cg_get_cell_fg(current_cell);
                cg_rgb_t cell_bg = cg_get_cell_bg(current_cell);
                // get the cell character
                cg_char c = cg_get_cell_char(current_cell);

                // move to the cell position
                _cg_term_move_to(j, i);

                // set the colours
                if (cg_compare_colour(cell_fg, current_fg) != 0)
                {
                    _cg_term_set_foreground_colour(cell_fg);
                    current_fg = cell_fg;
                }
                if (cg_compare_colour(cell_bg, current_bg) != 0)
                {
                    _cg_term_set_background_colour(cell_bg);
                    current_bg = cell_bg;
                }

                // write the character
                _cg_term_write_char(c);
            }
        }
    }

    _cg_term_flush_command_buffer(_cg_buffer);

    _cg_show_cursor();
}

void cg_clear_canvas()
{
    cg_background(default_bg_colour);
}

void cg_point(cg_uint x1, cg_uint y1, cg_char c)
{
    if (x1 >= canvas_current->width || y1 >= canvas_current->height || x1 < 0 || y1 < 0)
    {
        return;
    }
    cg_cell_t *cell = cg_get_cell(canvas_current, x1, y1);
    cg_set_cell_char(cell, c);
    cg_set_cell_bg(cell, background_colour);
    cg_set_cell_fg(cell, stroke_colour);
    // wprintf(L"\033[%lu;%luf", y1, x1);
    // wprintf(L"%c", c);
}

void cg_line(cg_uint x1, cg_uint y1, cg_uint x2, cg_uint y2)
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
            cg_point(x1, y, L'█');
        }
    }
    else
    {
        double slope = (y2 - y1) / (x2 - x1);
        for (cg_uint x = x1; x <= x2; x++)
        {
            cg_uint y = y1 + (slope * x);
            cg_point(x, y, L'█');
        }
    }
}

void cg_rect(cg_uint x1, cg_uint y1, cg_uint width, cg_uint height)
{
    cg_line(x1, y1, x1 + width, y1);
    cg_line(x1 + width, y1, x1 + width, y1 + height);
    cg_line(x1 + width, y1 + height, x1, y1 + height);
    cg_line(x1, y1, x1, y1 + height);
}

void cg_text(cg_char *t, cg_uint x, cg_uint y)
{
    cg_uint len = wcslen(t);
    if (len > 0)
    {
        for (cg_uint i = 0; i < len; i++)
        {
            cg_point(x + i, y, t[i]);
        }
    }
}

char cg_get_key_pressed_char()
{
    return _cg_gfx_context->key_pressed;
}

int cg_is_key_pressed(char c)
{
    return _cg_gfx_context->key_pressed == c;
}

int cg_create_graphics(cg_uint w, cg_uint h)
{
    // allocate the graphics context
    _cg_gfx_context = (_cg_graphics_context_t *)_CG_CALLOC(1, sizeof(_cg_graphics_context_t));

    if (_cg_gfx_context == NULL)
    {
        wprintf(L"FATAL Error: Unable to allocate graphics context.\n");
        return -1;
    }

    clock_gettime(CLOCK_MONOTONIC, &(_cg_gfx_context->start_time));
    _cg_gfx_context->prev_time = _cg_gfx_context->start_time;
    _cg_gfx_context->current_time = _cg_gfx_context->start_time;

    _cg_gfx_context->delta_time_ideal = 1000000 / _fps; // in microseconds

    // init random numbers
    srand(time(NULL));

    // create the command buffer
    if (_cg_term_create_command_buffer(&_cg_buffer) == -1)
    {
        wprintf(L"FATAL Error: Unable to create command buffer.\n");
        return -1;
    }

    // enable raw mode for terminal
    _cg_term_enable_raw_mode();

    // get the window size
    int rows, cols;
    if (w == 0 || h == 0)
    {
        if (_cg_get_window_size(&rows, &cols) == -1)
        {
            wprintf(L"FATAL Error: Unable to get window size.\n");
            return -1;
        }
    }
    else
    {
        rows = h;
        cols = w;
    }

    // init locale for terminal, and wide output
    setlocale(LC_ALL, "");
    fwide(stdout, 1);

    // if there is no canvas created, create a default one
    if (canvas_current == NULL)
    {
        cg_create_canvas(cols, rows);

        cg_background(default_bg_colour);
        cg_set_colour(default_fg_colour);

        // swap out the current canvas,
        // make the current canvas dirty with a foreground colour so that
        // it will not match with the previous canvas in the draw call
        // then draw the initial canvas
        cg_swap_canvas();
        cg_background(default_fg_colour);
        cg_swap_canvas();
        
        // now draw the canvas
        cg_show_canvas();

        // swap out the current canvas, and set the default background and foreground
        // to restore the initial state in both back and front canvas
        cg_swap_canvas();
        cg_background(default_bg_colour);
        cg_set_colour(default_fg_colour);
    }

    // set default background and forground
    _cg_term_reset();
    _cg_term_set_foreground_colour(default_fg_colour);

    cg_cls();
    cg_home();

    return 0;
}

int cg_create_graphics_fullscreen()
{
    return cg_create_graphics(0, 0);
}

int cg_begin_draw()
{
    _cg_gfx_context->dt = _diff_time_micros(_cg_gfx_context->current_time, _cg_gfx_context->prev_time);

    _cg_read_key();

    // if the key pressed is ESC, then return -1 to exit
    if (_cg_gfx_context->key_pressed == 27)
    {
        return -1;
    }

    return 0;
}

void cg_end_draw()
{
    // show the canvas
    cg_show_canvas();

    // how much time spent
    clock_gettime(CLOCK_MONOTONIC, &(_cg_gfx_context->after_draw_time));
    cg_uint dt_done = _diff_time_micros(_cg_gfx_context->after_draw_time, _cg_gfx_context->current_time);
    // wprintf(L"Delta ideal %lu, Delta done %lu\n", delta_time_ideal, dt_done);
    if (_cg_gfx_context->delta_time_ideal > dt_done)
    {
        struct timespec dt_diff, dt_diff_rem;
        dt_diff.tv_sec = 0;
        dt_diff.tv_nsec = (_cg_gfx_context->delta_time_ideal - dt_done) * 1000;
        // sleep for the difference
        //  wprintf(L"sleep for -> [%ld]nanos\n", dt_diff.tv_nsec);
        nanosleep(&dt_diff, &dt_diff_rem);
    }

    _cg_gfx_context->prev_time = _cg_gfx_context->current_time;
    clock_gettime(CLOCK_MONOTONIC, &(_cg_gfx_context->current_time));
    // dt_done = _diff_time_micros(current_time, prev_time);
    // wprintf(L"After sleep: Delta ideal %lu, Delta done %lu\n", delta_time_ideal, dt_done);

    // swap canvas
    cg_swap_canvas();
}

void cg_destroy_graphics()
{
    // free the graphics context
    if (_cg_gfx_context != NULL)
    {
        _CG_FREE(_cg_gfx_context);
    }

    // flush the command buffer
    _cg_term_flush_command_buffer(_cg_buffer);

    // dispose of the command buffer
    _cg_term_dispose_command_buffer(_cg_buffer);
}

cg_uint cg_get_deltatime_micros()
{
    return _cg_gfx_context->dt;
}

cg_uint cg_get_deltatime_millis()
{
    return _cg_gfx_context->dt / 1000;
}

#endif //__CONGFX_H__
