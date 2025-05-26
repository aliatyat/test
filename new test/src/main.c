// src/main.c
#include "../includes/game.h"

void put_pixel(int x, int y, int color, t_game *game)
{
    if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
        return;
    int index = y * game->size_line + x * game->bpp / 8;
    game->data[index] = color & 0xFF;
    game->data[index + 1] = (color >> 8) & 0xFF;
    game->data[index + 2] = (color >> 16) & 0xFF;
}

void clear_image(t_game *game)
{
    int y = 0;
    while (y < HEIGHT)
    {
        int x = 0;
        while (x < WIDTH)
        {
            if (y < HEIGHT / 2)
                put_pixel(x, y, game->ceiling_color, game);
            else
                put_pixel(x, y, game->floor_color, game);
            x++;
        }
        y++;
    }
}

void draw_map(t_game *game)
{
    if (!DEBUG)
        return;
    int y = 0;
    while (y < game->rows)
    {
        int x = 0;
        while (x < game->columns)
        {
            if (game->map[y][x] == '1')
            {
                int i = 0;
                while (i < BLOCK)
                {
                    int j = 0;
                    while (j < BLOCK)
                    {
                        put_pixel(x * BLOCK + j, y * BLOCK + i, 0x8B4513, game);
                        j++;
                    }
                    i++;
                }
            }
            x++;
        }
        y++;
    }
}

float distance(float x, float y)
{
    return sqrt(x * x + y * y);
}

float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
{
    float delta_x = x2 - x1;
    float delta_y = y2 - y1;
    float angle = atan2(delta_y, delta_x) - game->player.angle;
    return distance(delta_x, delta_y) * cos(angle);
}

bool touch(float px, float py, t_game *game)
{
    int x = (int)(px / BLOCK);
    int y = (int)(py / BLOCK);
    if (x < 0 || x >= game->columns || y < 0 || y >= game->rows)
        return true;
    return game->map[y][x] == '1';
}

void error_msg(t_game *game, char *error_msg)
{
    ft_putstr_fd("Error\n", STDERR_FILENO);
    ft_putendl_fd(error_msg, STDERR_FILENO);
    if (game->map)
        free_map(game->map);
    if (game->tex_no.img)
        mlx_destroy_image(game->mlx, game->tex_no.img);
    if (game->tex_so.img)
        mlx_destroy_image(game->mlx, game->tex_so.img);
    if (game->tex_we.img)
        mlx_destroy_image(game->mlx, game->tex_we.img);
    if (game->tex_ea.img)
        mlx_destroy_image(game->mlx, game->tex_ea.img);
    if (game->img)
        mlx_destroy_image(game->mlx, game->img);
    if (game->win)
        mlx_destroy_window(game->mlx, game->win);
    if (game->mlx)
        free(game->mlx);
    exit(EXIT_FAILURE);
}

void draw_line(t_player *player, t_game *game, float start_x, int i)
{
    float cos_angle = cos(start_x);
    float sin_angle = sin(start_x);
    float ray_x = player->x;
    float ray_y = player->y;
    float step = 0.1;
    bool hit = false;
    t_texture *texture = &game->tex_no;
    float wall_x = 0;

    while (!hit)
    {
        ray_x += cos_angle * step;
        ray_y += sin_angle * step;
        if (DEBUG)
            put_pixel(ray_x, ray_y, 0x0000FF, game);
        if (touch(ray_x, ray_y, game))
        {
            hit = true;
            float dx = ray_x - player->x;
            float dy = ray_y - player->y;
            float dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
            float height = (BLOCK / dist) * (WIDTH / 2);
            int start_y = (HEIGHT - (int)height) / 2;
            if (start_y < 0)
                start_y = 0;
            int end_y = start_y + (int)height;
            if (end_y > HEIGHT)
                end_y = HEIGHT;

            // Determine wall side and texture
            int map_x = (int)(ray_x / BLOCK);
            int map_y = (int)(ray_y / BLOCK);
            float side_dist_x = (map_x + (dx > 0 ? 1 : 0) - ray_x / BLOCK) * BLOCK / cos_angle;
            float side_dist_y = (map_y + (dy > 0 ? 1 : 0) - ray_y / BLOCK) * BLOCK / sin_angle;
            if (fabs(side_dist_x) < fabs(side_dist_y))
            {
                wall_x = ray_y / BLOCK - map_y;
                texture = (dx > 0) ? &game->tex_ea : &game->tex_we;
            }
            else
            {
                wall_x = ray_x / BLOCK - map_x;
                texture = (dy > 0) ? &game->tex_so : &game->tex_no;
            }
            wall_x -= floor(wall_x);

            // Draw textured wall
            int tex_x = (int)(wall_x * texture->width);
            int y = start_y;
            while (y < end_y)
            {
                float tex_y = (y - (HEIGHT - height) / 2) * texture->height / height;
                int tex_offset = ((int)tex_y * texture->size_line) + (tex_x * texture->bpp / 8);
                int color = *(int *)(texture->data + tex_offset);
                put_pixel(i, y, color, game);
                y++;
            }
        }
    }
}

int draw_loop(t_game *game)
{
    move_player(&game->player, game);
    clear_image(game);
    if (DEBUG)
    {
        draw_map(game);
        put_pixel(game->player.x, game->player.y, 0x00FF00, game);
    }
    float fraction = PI / 3 / WIDTH;
    float start_x = game->player.angle - PI / 6;
    int i = 0;
    while (i < WIDTH)
    {
        draw_line(&game->player, game, start_x, i);
        start_x += fraction;
        i++;
    }
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
    return 0;
}

int quit_game(t_game *game)
{
    error_msg(game, "Game closed");
    return 0;
}

void init_game(t_game *game, char *map_file)
{
    ft_bzero(game, sizeof(t_game));
    get_map(map_file, game);
    game->mlx = mlx_init();
    if (!game->mlx)
        error_msg(game, MLX_INIT_ERR);
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Cub3D");
    if (!game->win)
        error_msg(game, MLX_NEW_WINDOW_ERR);
    game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    if (!game->img)
        error_msg(game, "Failed to create image");
    game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
    if (!game->data)
        error_msg(game, "Failed to get image data");
}

int main(int ac, char **av)
{
    t_game game;
    if (ac != 2)
        error_msg(&game, INVALID_NBR_ARGS);
    init_game(&game, av[1]);
    if (DEBUG)
    {
        int x = 0;
        while (x < game.rows)
        {
            ft_printf("%s\n", game.map[x]);
            x++;
        }
    }
    mlx_hook(game.win, KeyPress, KeyPressMask, key_press, &game.player);
    mlx_hook(game.win, KeyRelease, KeyReleaseMask, key_release, &game.player);
    mlx_hook(game.win, DestroyNotify, StructureNotifyMask, quit_game, &game);
    mlx_loop_hook(game.mlx, draw_loop, &game);
    mlx_loop(game.mlx);
    return 0;
}