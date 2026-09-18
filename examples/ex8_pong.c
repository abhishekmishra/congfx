/* A small two-player Pong game for ConGfx.
 *
 * Left paddle: W/S        Right paddle: Up/Down
 * R resets the scores and ball.  Escape quits.
 */
#define CONGFX_IMPLEMENTATION
#include "congfx.h"

#define PADDLE_HEIGHT 5
#define PADDLE_X 2
#define BALL_SPEED_X 24.0L
#define BALL_SPEED_Y 12.0L

typedef struct
{
    cg_number x;
    cg_number y;
    cg_number vx;
    cg_number vy;
} ball_t;

static int clamp_int(int value, int minimum, int maximum)
{
    if (value < minimum)
        return minimum;
    if (value > maximum)
        return maximum;
    return value;
}

static void reset_ball(ball_t *ball, int direction)
{
    ball->x = (cg_number)(width / 2);
    ball->y = (cg_number)(height / 2);
    ball->vx = BALL_SPEED_X * direction;
    ball->vy = BALL_SPEED_Y;
}

static void draw_paddle(int x, int y)
{
    for (int row = 0; row < PADDLE_HEIGHT; row++)
        cg_point_char((cg_uint)x, (cg_uint)(y + row), '#');
}

static void draw_centre_line(int top, int bottom)
{
    for (int y = top; y <= bottom; y += 2)
        cg_point_char(width / 2, (cg_uint)y, '|');
}

static void draw_ball(const ball_t *ball)
{
    const cg_uint x = (cg_uint)ball->x;
    const cg_uint y = (cg_uint)ball->y;

    cg_point_char(x, y, 'O');
    cg_point_char(x + 1, y, 'O');
    cg_point_char(x, y + 1, 'O');
    cg_point_char(x + 1, y + 1, 'O');
}

int main(void)
{
    int err;
    int left_paddle;
    int right_paddle;
    int left_score = 0;
    int right_score = 0;
    int direction = 1;
    const int top = 2;
    ball_t ball;
    char score_text[32];

    cg_frame_rate(60);
    err = cg_create_graphics_fullscreen();
    if (err != 0)
        return err;

    if (width < 32 || height < 12)
    {
        cg_begin_draw();
        cg_clear_canvas();
        cg_text("Pong needs a terminal at least 32 columns by 12 rows.", 0, 0);
        cg_text("Press ESC to exit.", 0, height > 1 ? height - 1 : 0);
        cg_end_draw();
        while (!cg_should_exit())
        {
            cg_begin_draw();
            cg_end_draw();
        }
        cg_destroy_graphics();
        return 0;
    }

    left_paddle = (int)(height / 2) - PADDLE_HEIGHT / 2;
    right_paddle = left_paddle;
    reset_ball(&ball, direction);

    while (!cg_should_exit())
    {
        const int bottom = (int)height - 3;
        const int paddle_max = bottom - PADDLE_HEIGHT + 1;
        cg_keyboard_input_t input;

        cg_begin_draw();

        while ((input = cg_get_key_pressed()).key != CG_KEY_NONE)
        {
            if (input.key == CG_KEY_UP)
                right_paddle--;
            else if (input.key == CG_KEY_DOWN)
                right_paddle++;
            else if (input.key == CG_KEY_ALPHANUM)
            {
                if (input.char_value == 'w' || input.char_value == 'W')
                    left_paddle--;
                else if (input.char_value == 's' || input.char_value == 'S')
                    left_paddle++;
                else if (input.char_value == 'r' || input.char_value == 'R')
                {
                    left_score = 0;
                    right_score = 0;
                    direction = 1;
                    reset_ball(&ball, direction);
                }
            }
        }

        left_paddle = clamp_int(left_paddle, top, paddle_max);
        right_paddle = clamp_int(right_paddle, top, paddle_max);

        /* Delta time is milliseconds; velocities are cells per second. */
        cg_number elapsed = (cg_number)cg_get_deltatime() / 1000.0L;
        ball.x += ball.vx * elapsed;
        ball.y += ball.vy * elapsed;

        if (ball.y <= top)
        {
            ball.y = top;
            ball.vy = -ball.vy;
        }
        else if (ball.y >= bottom)
        {
            ball.y = bottom;
            ball.vy = -ball.vy;
        }

        if (ball.vx < 0 && ball.x <= PADDLE_X + 1 && ball.x >= PADDLE_X &&
            ball.y >= left_paddle && ball.y < left_paddle + PADDLE_HEIGHT)
        {
            ball.x = PADDLE_X + 1;
            ball.vx = -ball.vx;
            ball.vy += (ball.y - (left_paddle + PADDLE_HEIGHT / 2)) * 2.0L;
        }
        else if (ball.vx > 0 && ball.x >= (cg_number)(width - PADDLE_X - 2) &&
                 ball.x <= (cg_number)(width - PADDLE_X - 1) &&
                 ball.y >= right_paddle && ball.y < right_paddle + PADDLE_HEIGHT)
        {
            ball.x = width - PADDLE_X - 2;
            ball.vx = -ball.vx;
            ball.vy += (ball.y - (right_paddle + PADDLE_HEIGHT / 2)) * 2.0L;
        }

        if (ball.x < 0)
        {
            right_score++;
            direction = 1;
            reset_ball(&ball, direction);
        }
        else if (ball.x >= width)
        {
            left_score++;
            direction = -1;
            reset_ball(&ball, direction);
        }

        cg_clear_canvas();
        cg_text("CONGFX: EXAMPLE #8: PONG", width / 2 - 13, 0);
        snprintf(score_text, sizeof(score_text), "%d  :  %d", left_score, right_score);
        cg_text(score_text, width / 2 - 3, 1);
        draw_centre_line(top, bottom);
        draw_paddle(PADDLE_X, left_paddle);
        draw_paddle((int)width - PADDLE_X - 1, right_paddle);
        draw_ball(&ball);
        cg_text("W/S: left paddle  Up/Down: right paddle  R: reset  ESC: quit", 0, height - 1);
        cg_end_draw();
    }

    cg_destroy_graphics();
    return 0;
}
