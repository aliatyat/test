#include "../includes/game.h"

// our own put_pixel

void put_pixel(int x, int y, int color, t_game *game)
{
    if(x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
        return;
    
    int index = y * game->size_line + x * game->bpp / 8;
    
    game->data[index] = color & 0xFF;
    game->data[index + 1] = (color >> 8) & 0xFF;
    game->data[index + 2] = (color >> 16) & 0xFF;
}

// our own clear_image
// void clear_image(t_game *game)
// {
//     for(int y = 0; y < HEIGHT; y++)
//         for(int x = 0; x < WIDTH; x++)
//             put_pixel(x, y, 0xFFFF00, game);
// }

void clear_image(t_game *game)
{
    int ceiling_color = 0x87CEEB; // Light blue for the ceiling
    int floor_color = 0x00FF00;   // green for the floor
    if (!DEBUG)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                if (y < HEIGHT / 2) // Top half for the ceiling
                    put_pixel(x, y, ceiling_color, game);
                else // Bottom half for the floor
                    put_pixel(x, y, floor_color, game);
            }
        }
    }
    else
    {
        for(int y = 0; y < HEIGHT; y++)
            for(int x = 0; x < WIDTH; x++)
                put_pixel(x, y, 0xFFFF00, game);

    }
}


// utils functions
void draw_square(int x, int y, int size, int color, t_game *game)
{
    if (!DEBUG)
    {
    for(int i = 0; i < size; i++)
        put_pixel(x + i, y, color, game);
    for(int i = 0; i < size; i++)
        put_pixel(x, y + i, color, game);
    for(int i = 0; i < size; i++)
        put_pixel(x + size, y + i, color, game);
    for(int i = 0; i < size; i++)
        put_pixel(x + i, y + size, color, game);
    }
    else
    {
        for(int i = 0; i < size; i++)
        put_pixel(x + i, y, 0x000ff, game);
    for(int i = 0; i < size; i++)
        put_pixel(x, y + i, 0x000ff, game);
    for(int i = 0; i < size; i++)
        put_pixel(x + size, y + i, 0x000ff, game);
    for(int i = 0; i < size; i++)
        put_pixel(x + i, y + size, 0x000ff, game);

    }
}

void draw_map(t_game *game)
{
    char **map = game->map;
    int color =  0x8B4513;
    for(int y = 0; map[y]; y++)
        for(int x = 0; map[y][x]; x++)
            if(map[y][x] == '1')
                draw_square(x * BLOCK, y * BLOCK, BLOCK, color, game);
}

// distance calculation functions
float distance(float x, float y){
    return sqrt(x * x + y * y);
}

float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
{
    float delta_x = x2 - x1;
    float delta_y = y2 - y1;
    float angle = atan2(delta_y, delta_x) - game->player.angle;
    float fix_dist = distance(delta_x, delta_y) * cos(angle);
    return fix_dist;
}

bool touch(float px, float py, t_game *game)
{
   int x = px / BLOCK;
int y = py / BLOCK;

    // if (px < 0 || py >= game->rows || px < 0 || py >= game->columns)
    // return true;  // treat out-of-bounds as a wall

if (game->map[y][x] == '1')
    return true;
return false;
}

// initialisation functions
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
        exit(0);
    char *temp = get_next_line(map_fd);
    while (temp)
    {
        counter++;
        free(temp);
        temp = get_next_line(map_fd);
    }
    close(map_fd);
    if (counter == 0)
       exit(0);
    game->rows = counter;
}

static void get_lines(char *map_file, t_game *game)
{
    int map_fd = open(map_file, O_RDONLY);
    if (map_fd == -1)
        error_msg(game, OPEN_MAP_FILE_ERR);
    int i = 0;
    while (i < game->rows)
        game->map[i++] = get_next_line(map_fd);
    game->map[i] = NULL;
    close(map_fd);
    i = 0;
    while (i < game->rows)
    {
        char *temp = game->map[i];
        game->map[i] = ft_strtrim(game->map[i], "\n");
        free(temp);
        if (!game->map[i])
            error_msg(game, MALLOC_ERR);
        i++;
    }
    game->columns = ft_strlen(game->map[0]);
}

void get_map1(char *map_file, t_game *game)
{
    if (!valid_extension(map_file))
        error_msg(game, INVALID_MAP_FILE);
    get_nbr_rows(map_file, game);
    game->map = malloc((game->rows + 1) * sizeof(char *));
    if (!game->map)
        error_msg(game, MALLOC_ERR);
    get_lines(map_file, game);
}

void init_game(t_game *game, char *map_file)
{
    get_map1(map_file, game);
    init_player(&game->player);
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Game");
    game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

// raycasting functions
void draw_line(t_player *player, t_game *game, float start_x, int i)
{
    float cos_angle = cos(start_x);
    float sin_angle = sin(start_x);
    float ray_x = player->x;
    float ray_y = player->y;

    while(!touch(ray_x, ray_y, game))
    {
        if(DEBUG)
            put_pixel(ray_x, ray_y, 0x0000FF, game);
        ray_x += cos_angle;
        ray_y += sin_angle;
    }
    if(!DEBUG)
    {
        float dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
        float height = (BLOCK / dist) * (WIDTH / 2);
        int start_y = (HEIGHT - height) / 2;
        int end = start_y + height;
        while(start_y < end)
        {
            put_pixel(i, start_y,  0x8B4513, game);
            start_y++;
        }
    }
}

int draw_loop(t_game *game)
{
    t_player *player = &game->player;
    move_player(player, game);
    clear_image(game);
    if(DEBUG)
    {
        draw_square(player->x, player->y, 10, 0x00FF00, game);
        draw_map(game);
    }
    float fraction = PI / 3 / WIDTH;
    float start_x = player->angle - PI / 6;
    int i = 0;
    while(i < WIDTH)
    {
        draw_line(player, game, start_x, i);
        start_x += fraction;
        i++;
    }
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
    return 0;
}


int	quit_game(t_game *game)
{
	//destroy(game);
	exit(EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}


int main(int ac, char **av)
{
    t_game game;
    // init
    init_game(&game, av[1]);
    // hooks
    int x = 0;
    while (x < game.rows)
    {
        printf("%s\n", game.map[x]);
        x++;
    }
    mlx_hook(game.win, 2, 1L<<0, key_press, &game.player);
    mlx_hook(game.win, 3, 1L<<1, key_release, &game.player);
    mlx_hook(game.win, 17 , (1L << 17),
		quit_game, &game);

    // draw loop
    mlx_loop_hook(game.mlx, draw_loop, &game);

    mlx_loop(game.mlx);
    return 0;
}