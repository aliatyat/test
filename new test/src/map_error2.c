// src/map_error2.c
#include "../includes/game.h"

static void set_player(t_game *game, int x, int y, char dir)
{
    game->player.x = x * BLOCK + BLOCK / 2;
    game->player.y = y * BLOCK + BLOCK / 2;
    if (dir == 'N')
        game->player.angle = 3 * PI / 2;
    else if (dir == 'S')
        game->player.angle = PI / 2;
    else if (dir == 'E')
        game->player.angle = 0;
    else if (dir == 'W')
        game->player.angle = PI;
    game->map[y][x] = '0';
}

static void check_player(t_game *game)
{
    int player_count = 0;
    int y = 0;
    while (y < game->rows)
    {
        int x = 0;
        while (x < game->columns)
        {
            if (ft_strchr("NSEW", game->map[y][x]))
            {
                set_player(game, x, y, game->map[y][x]);
                player_count++;
            }
            else if (!ft_strchr("01 ", game->map[y][x]))
                error_msg(game, INVALID_ENTITY);
            x++;
        }
        y++;
    }
    // if (player_count != 1)
    //     error_msg(game, INVALID_PLAYER);
}

static void flood_fill(char **map, int x, int y, int rows, int cols)
{
    if (x < 0 || x >= cols || y < 0 || y >= rows || map[y][x] == '1' || map[y][x] == 'F' || map[y][x] == ' ')
        return;
    map[y][x] = 'F';
    flood_fill(map, x + 1, y, rows, cols);
    flood_fill(map, x - 1, y, rows, cols);
    flood_fill(map, x, y + 1, rows, cols);
    flood_fill(map, x, y - 1, rows, cols);
}

static char **copy_map(char **map, int rows, int cols)
{
    char **copy = malloc(sizeof(char *) * (rows + 1));
    if (!copy)
        return NULL;
    int i = 0;
    while (i < rows)
    {
        copy[i] = malloc(cols + 1);
        if (!copy[i])
        {
            while (i > 0)
                free(copy[--i]);
            free(copy);
            return NULL;
        }
        ft_memcpy(copy[i], map[i], cols);
        copy[i][cols] = '\0';
        i++;
    }
    copy[i] = NULL;
    if (DEBUG)
    {
        i = 0;
        while (i < rows)
            ft_printf("Copy Map[%d]: %s\n", i, copy[i++]);
    }
    return copy;
}

static void check_enclosure(t_game *game)
{
    char **map_copy = copy_map(game->map, game->rows, game->columns);
    if (!map_copy)
        error_msg(game, MALLOC_ERR);
    int start_x = (int)(game->player.x / BLOCK);
    int start_y = (int)(game->player.y / BLOCK);
    flood_fill(map_copy, start_x, start_y, game->rows, game->columns);
    int y = 0;
    while (y < game->rows)
    {
        int x = 0;
        while (x < game->columns)
        {
            if (map_copy[y][x] == '0')
            {
                ft_printf("Unclosed at (%d, %d): %c\n", x, y, map_copy[y][x]);
                free_map(map_copy);
                error_msg(game, MAP_NOT_CLOSED);
            }
            x++;
        }
        y++;
    }
    if (DEBUG)
    {
        y = 0;
        while (y < game->rows)
            ft_printf("Flooded Map[%d]: %s\n", y, map_copy[y++]);
    }
    free_map(map_copy);
}

void validate_map(t_game *game)
{
    int y = 0;
    while (y < game->rows)
    {
        int x = 0;
        while (x < game->columns)
        {
            if ((y == 0 || y == game->rows - 1 || x == 0 || x == game->columns - 1) &&
                game->map[y][x] != '1' && game->map[y][x] != ' ')
            {
                ft_printf("Invalid border at (%d, %d): %c\n", x, y, game->map[y][x]);
                error_msg(game, MAP_NOT_CLOSED);
            }
            x++;
        }
        y++;
    }
    if (DEBUG)
    {
        y = 0;
        while (y < game->rows)
            ft_printf("Validated Map[%d]: %s\n", y, game->map[y++]);
    }
    check_player(game);
    check_enclosure(game);
}