#include "../includes/game.h"

#define CEILING_COLOR 0x87CEEB
#define FLOOR_COLOR   0x00FF00
#define WALL_COLOR    0x8B4513



void	error_msg(t_game *game, char *error_msg)
{
	//destroy(game);
	ft_putstr_fd("Error\n", STDERR_FILENO);
	ft_putendl_fd(error_msg, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

static int valid_extension(char *map_file)
{
    size_t i = ft_strlen(map_file) - 4;
    return (ft_strncmp(".cub", &map_file[i], 4) == 0);
}

static void get_nbr_rows(char *map_file, t_game *game)
{
    int counter = 0;
    int map_fd = open(map_file, O_RDONLY);
    if (map_fd == -1)
        error_msg(game, OPEN_MAP_FILE_ERR);
    char *temp = get_next_line(map_fd);
    while (temp)
    {
        counter++;
        free(temp);
        temp = get_next_line(map_fd);
    }
    close(map_fd);
    if (counter == 0)
        error_msg(game, EMPITY_MAP_FILE);
    game->map.rows = counter;
}

static void get_lines(char *map_file, t_game *game)
{
    int map_fd = open(map_file, O_RDONLY);
    if (map_fd == -1)
        error_msg(game, OPEN_MAP_FILE_ERR);
    int i = 0;
    while (i < game->map.rows)
        game->map.map[i++] = get_next_line(map_fd);
    game->map.map[i] = NULL;
    close(map_fd);
    i = 0;
    while (i < game->map.rows)
    {
        char *temp = game->map.map[i];
        game->map.map[i] = ft_strtrim(game->map.map[i], "\n");
        free(temp);
        if (!game->map.map[i])
            error_msg(game, MALLOC_ERR);
        i++;
    }
    game->map.columns = ft_strlen(game->map.map[0]);
}

void get_map(char *map_file, t_game *game)
{
    if (!valid_extension(map_file))
        error_msg(game, INVALID_MAP_FILE);
    get_nbr_rows(map_file, game);
    game->map.map = malloc((game->map.rows + 1) * sizeof(char *));
    if (!game->map.map)
        error_msg(game, MALLOC_ERR);
    get_lines(map_file, game);
}

// void put_pixel(int x, int y, int color, t_game *game)
// {
//     if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
//         return;
//     int index = y * game->size_line + x * (game->bpp / 8);
//     game->data[index] = color & 0xFF;
//     game->data[index + 1] = (color >> 8) & 0xFF;
//     game->data[index + 2] = (color >> 16) & 0xFF;
// }
void put_pixel(int x, int y, int color, t_game *game)
{
    if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
        return;
    if (!game->data) {
        fprintf(stderr, "Error: game->data is NULL in put_pixel\n");
        return;
    }
    int index = y * game->size_line + x * (game->bpp / 8);
    // Optional: check if index is within buffer size
    int max_index = HEIGHT * game->size_line;
    if (index + 2 >= max_index) {
        fprintf(stderr, "Error: put_pixel index out of bounds: %d (max %d)\n", index, max_index);
        return;
    }
    game->data[index] = color & 0xFF;
    game->data[index + 1] = (color >> 8) & 0xFF;
    game->data[index + 2] = (color >> 16) & 0xFF;
}
void clear_image(t_game *game)
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (y < HEIGHT / 2)
                put_pixel(x, y, CEILING_COLOR, game);
            else
                put_pixel(x, y, FLOOR_COLOR, game);
        }
    }
}

void draw_square(int x, int y, int size, int color, t_game *game)
{
    for (int i = 0; i < size; i++)
    {
        put_pixel(x + i, y, color, game);
        put_pixel(x + i, y + size, color, game);
        put_pixel(x, y + i, color, game);
        put_pixel(x + size, y + i, color, game);
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
    int x = px / BLOCK;
    int y = py / BLOCK;

    if (y < 0 || y >= game->map.rows || x < 0 || x >= game->map.columns)
        return true; // treat out-of-bounds as wall

    return (game->map.map[y][x] == '1');
}

void destroy_game(t_game *game)
{
    for (int i = 0; i < game->map.rows; i++)
        free(game->map.map[i]);
    free(game->map.map);
    mlx_destroy_image(game->mlx, game->img);
    mlx_destroy_window(game->mlx, game->win);
    mlx_destroy_display(game->mlx);
    free(game->mlx);
}

void init_game(t_game *game)
{
    init_player(&game->player);
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Game");
    game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

void draw_line(t_player *player, t_game *game, float start_x, int i)
{
    float cos_angle = cos(start_x);
    float sin_angle = sin(start_x);
    float ray_x = player->x;
    float ray_y = player->y;
    while (!touch(ray_x, ray_y, game))
    {
        ray_x += cos_angle * 0.1f;
        ray_y += sin_angle * 0.1f;
    }
    float dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
    float height = (BLOCK / dist) * (WIDTH / 2);
    int start_y = (HEIGHT - height) / 2;
    int end = start_y + height;
    while (start_y < end)
        put_pixel(i, start_y++, WALL_COLOR, game);
}

int draw_loop(t_game *game)
{
    t_player *player = &game->player;
    move_player(player, game);
    clear_image(game);
    float fraction = PI / 3 / WIDTH;
    float start_x = player->angle - PI / 6;
    for (int i = 0; i < WIDTH; i++, start_x += fraction)
        draw_line(player, game, start_x, i);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
    return 0;
}

int quit_game(t_game *game)
{
   // destroy_game(game);
    exit(EXIT_SUCCESS);
    return 0;
}

int main(int ac, char **av)
{
    if (ac < 2)
    {
        ft_putstr_fd("Usage: ./cub3d map.cub\n", 2);
        return EXIT_FAILURE;
    }
    t_game game;
    get_map(av[1], &game);
    init_game(&game);
  
    int i = 0;
    while (game.map.map[i])
    {
        printf("%s\n", game.map.map[i]);
        i++;
    }

    mlx_hook(game.win, 2, 1L << 0, key_press, &game.player);
    mlx_hook(game.win, 3, 1L << 1, key_release, &game.player);
    mlx_hook(game.win, 17, (1L << 17), quit_game, &game);
    mlx_loop_hook(game.mlx, draw_loop, &game);
    mlx_loop(game.mlx);
    return 0;
}
