/* A small one-player Snake game for ConGfx.
 *
 * Arrow keys move the snake. R starts a new game. Escape quits.
 */
#define CONGFX_IMPLEMENTATION
#include "congfx.h"

#define START_LENGTH 4
#define STEP_MILLIS 120

typedef struct
{
    int x;
    int y;
} cell_t;

typedef enum
{
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT
} direction_t;

static int snake_contains(const cell_t *snake, int length, cell_t cell)
{
    for (int i = 0; i < length; i++)
        if (snake[i].x == cell.x && snake[i].y == cell.y)
            return 1;
    return 0;
}

static int directions_are_opposites(direction_t first, direction_t second)
{
    return (first == DIRECTION_UP && second == DIRECTION_DOWN) ||
           (first == DIRECTION_DOWN && second == DIRECTION_UP) ||
           (first == DIRECTION_LEFT && second == DIRECTION_RIGHT) ||
           (first == DIRECTION_RIGHT && second == DIRECTION_LEFT);
}

static int place_food(const cell_t *snake, int length, int board_bottom, cell_t *food)
{
    const int board_cells = (int)width * (board_bottom - 1);

    if (length >= board_cells)
        return 0;

    do
    {
        food->x = cg_rand_int(0, (int)width - 1);
        food->y = cg_rand_int(2, board_bottom);
    } while (snake_contains(snake, length, *food));

    return 1;
}

static void reset_game(cell_t *snake, int *length, direction_t *direction,
                       cell_t *food, int board_bottom, int *game_over, int *won)
{
    const int center_x = (int)width / 2;
    const int center_y = (2 + board_bottom) / 2;

    *length = START_LENGTH;
    *direction = DIRECTION_RIGHT;
    *game_over = 0;
    *won = 0;

    for (int i = 0; i < *length; i++)
        snake[i] = (cell_t){center_x - i, center_y};

    if (!place_food(snake, *length, board_bottom, food))
        *won = 1;
}

int main(void)
{
    const int board_top = 2;
    int board_bottom;
    int capacity;
    int length;
    int game_over;
    int won;
    cg_uint accumulated_time = 0;
    direction_t direction;
    cell_t food;
    cell_t *snake;
    char status[80];

    cg_frame_rate(60);
    int err = cg_create_graphics_fullscreen();
    if (err != 0)
        return err;

    if (width < 24 || height < 10)
    {
        while (!cg_should_exit())
        {
            cg_begin_draw();
            cg_clear_canvas();
            cg_text("Snake needs a terminal at least 24 columns by 10 rows.", 0, 0);
            cg_text("Press ESC to exit.", 0, height > 1 ? height - 1 : 0);
            cg_end_draw();
        }
        cg_destroy_graphics();
        return 0;
    }

    board_bottom = (int)height - 3;
    capacity = (int)width * (board_bottom - board_top + 1);
    snake = calloc((size_t)capacity, sizeof(*snake));
    if (snake == NULL)
    {
        cg_destroy_graphics();
        return 1;
    }

    reset_game(snake, &length, &direction, &food, board_bottom, &game_over, &won);

    while (!cg_should_exit())
    {
        cg_keyboard_input_t input;
        direction_t requested_direction = direction;

        cg_begin_draw();

        while ((input = cg_get_key_pressed()).key != CG_KEY_NONE)
        {
            if (input.key == CG_KEY_UP)
                requested_direction = DIRECTION_UP;
            else if (input.key == CG_KEY_DOWN)
                requested_direction = DIRECTION_DOWN;
            else if (input.key == CG_KEY_LEFT)
                requested_direction = DIRECTION_LEFT;
            else if (input.key == CG_KEY_RIGHT)
                requested_direction = DIRECTION_RIGHT;
            else if (input.key == CG_KEY_ALPHANUM &&
                     (input.char_value == 'r' || input.char_value == 'R'))
            {
                reset_game(snake, &length, &direction, &food, board_bottom, &game_over, &won);
                accumulated_time = 0;
                requested_direction = direction;
            }
        }

        if (!directions_are_opposites(direction, requested_direction))
            direction = requested_direction;

        accumulated_time += cg_get_deltatime();
        while (accumulated_time >= STEP_MILLIS && !game_over && !won)
        {
            cell_t next = snake[0];
            int ate_food;

            accumulated_time -= STEP_MILLIS;
            if (direction == DIRECTION_UP)
                next.y--;
            else if (direction == DIRECTION_DOWN)
                next.y++;
            else if (direction == DIRECTION_LEFT)
                next.x--;
            else
                next.x++;

            ate_food = next.x == food.x && next.y == food.y;
            if (next.x < 0 || next.x >= (int)width || next.y < board_top ||
                next.y > board_bottom || snake_contains(snake, length - (ate_food ? 0 : 1), next))
            {
                game_over = 1;
                break;
            }

            for (int i = length - 1; i > 0; i--)
                snake[i] = snake[i - 1];
            snake[0] = next;

            if (ate_food)
            {
                length++;
                if (!place_food(snake, length, board_bottom, &food))
                    won = 1;
            }
        }

        cg_clear_canvas();
        snprintf(status, sizeof(status), "CONGFX: EXAMPLE #9: SNAKE    Score: %d", length - START_LENGTH);
        cg_text(status, 0, 0);
        cg_text("Arrow keys: move    R: restart    ESC: quit", 0, 1);

        cg_set_colour((cg_rgb_t){255, 80, 80});
        cg_point_char((cg_uint)food.x, (cg_uint)food.y, '*');
        cg_set_colour((cg_rgb_t){80, 220, 120});
        for (int i = 1; i < length; i++)
            cg_point_char((cg_uint)snake[i].x, (cg_uint)snake[i].y, 'o');
        cg_set_colour((cg_rgb_t){255, 255, 255});
        cg_point_char((cg_uint)snake[0].x, (cg_uint)snake[0].y, '@');

        if (game_over)
            cg_text("Game over! Press R to play again.", 0, height / 2);
        else if (won)
            cg_text("You filled the board! Press R to play again.", 0, height / 2);

        cg_text("Press ESC to exit", 0, height - 1);
        cg_end_draw();
    }

    free(snake);
    cg_destroy_graphics();
    return 0;
}
