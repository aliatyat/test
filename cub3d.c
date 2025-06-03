#include "MLX42/MLX42.h"
#include <stdio.h>
#include <math.h>

#define WIDTH 1000
#define HEIGHT 840
#define MOVE_SPEED 0.05
#define ROT_SPEED 0.03

#define CEILING_COLOR 0x0000FFFF
#define FLOOR_COLOR   0x00FF00FF

char map[10][11] = {
    "1111111111",
    "1020E00001",
    "1030000101",
    "1040000101",
    "1010110101",
    "1010000101",
    "1013000101",
    "1000000001",
    "1111111101",
    "1111111111"
};

typedef struct s_game {
    mlx_t *mlx;
    mlx_image_t *img;
    mlx_texture_t *textures[4]; // textures[1] → wall 1, textures[2] → wall 2, etc.
    double posX, posY;
    double dirX, dirY;
    double planeX, planeY;
} t_game;
//cc thisrdraycast.c MLX42/build/libmlx42.a -IMLX42/include -ldl -lglfw -pthread -lm
void set_player_start(t_game *game)
{
    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            if (map[y][x] == 'N' || map[y][x] == 'S' || map[y][x] == 'E' || map[y][x] == 'W')
            {
                game->posX = x + 0.5;
                game->posY = y + 0.5;
                

                if (map[y][x] == 'N')
                { game->dirX = 0; game->dirY = -1; game->planeX = 0.66; game->planeY = 0; }
                if (map[y][x] == 'S')
                { game->dirX = 0; game->dirY = 1; game->planeX = -0.66; game->planeY = 0; }
                if (map[y][x] == 'E')
                { game->dirX = 1; game->dirY = 0; game->planeX = 0; game->planeY = 0.66; }
                if (map[y][x] == 'W')
                { game->dirX = -1; game->dirY = 0; game->planeX = 0; game->planeY = -0.66; }

                map[y][x] = '0';
                return;
            }
        }
    }
}

void render(t_game *game)
{
    mlx_image_t *img = game->img;
    mlx_delete_image(game->mlx, img);
    game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    for (int x = 0; x < WIDTH; x++)
    {
        double cameraX = 2 * x / (double)WIDTH - 1;
        double rayDirX = game->dirX + game->planeX * cameraX;
        double rayDirY = game->dirY + game->planeY * cameraX;

        int mapX = (int)game->posX;
        int mapY = (int)game->posY;

        double sideDistX, sideDistY;
        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double perpWallDist;
        int stepX, stepY, hit = 0, side;
        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (game->posX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - game->posX) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (game->posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - game->posY) * deltaDistY;
        }

        while (hit == 0)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (map[mapY][mapX] != '0')
                hit = 1;
        }

        if (side == 0)
            perpWallDist = (mapX - game->posX + (1 - stepX) / 2) / rayDirX;
        else
            perpWallDist = (mapY - game->posY + (1 - stepY) / 2) / rayDirY;

        int lineHeight = (int)(HEIGHT / perpWallDist);
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT) drawEnd = HEIGHT - 1;

        // Select texture based on wall type
        int wall_type = (map[mapY][mapX] - '0') % 4;
        mlx_texture_t *current_tex = game->textures[wall_type];

        double wallX;
        if (side == 0)
            wallX = game->posY + perpWallDist * rayDirY;
        else
            wallX = game->posX + perpWallDist * rayDirX;
        wallX -= floor(wallX);

        int texX = (int)(wallX * (double)current_tex->width);
        if (side == 0 && rayDirX > 0) texX = current_tex->width - texX - 1;
        if (side == 1 && rayDirY < 0) texX = current_tex->width - texX - 1;

        // Draw ceiling
        for (int y = 0; y < drawStart; y++)
            mlx_put_pixel(game->img, x, y, CEILING_COLOR);

        // Draw floor
        for (int y = drawEnd; y < HEIGHT; y++)
            mlx_put_pixel(game->img, x, y, FLOOR_COLOR);

        // Draw wall
        for (int y = drawStart; y < drawEnd; y++)
        {
            int d = y * 256 - HEIGHT * 128 + lineHeight * 128;
            int texY = ((d * current_tex->height) / lineHeight) / 256;

            uint8_t *pixel = &current_tex->pixels[(texY * current_tex->width + texX) * 4];
            uint8_t r = pixel[0];
            uint8_t g = pixel[1];
            uint8_t b = pixel[2];
            uint8_t a = pixel[3];

            uint32_t color = (r << 24) | (g << 16) | (b << 8) | a;
            mlx_put_pixel(game->img, x, y, color);
        }
    }

    mlx_image_to_window(game->mlx, game->img, 0, 0);
}

void hook(void *param)
{
    t_game *game = param;

    if (mlx_is_key_down(game->mlx, MLX_KEY_W))
    {

        if (map[(int)(game->posY)][(int)(game->posX + game->dirX * MOVE_SPEED)] == '0')
            game->posX += game->dirX * MOVE_SPEED;
        if (map[(int)(game->posY + game->dirY * MOVE_SPEED)][(int)(game->posX)] == '0')
            game->posY += game->dirY * MOVE_SPEED;
        if (map[(int)(game->posY)][(int)(game->posX)] == '1')
            game->posX += 0;
        if (map[(int)(game->posY)][(int)(game->posX)] == '1')
            game->posY += 0;
        
    }
    if (mlx_is_key_down(game->mlx, MLX_KEY_S))
    {
        if (map[(int)(game->posY)][(int)(game->posX - game->dirX * MOVE_SPEED)] == '0')
            game->posX -= game->dirX * MOVE_SPEED;
        if (map[(int)(game->posY - game->dirY * MOVE_SPEED)][(int)(game->posX)] == '0')
            game->posY -= game->dirY * MOVE_SPEED;
        if (map[(int)(game->posY)][(int)(game->posX)] == '1')
            game->posX += 0;
        if (map[(int)(game->posY)][(int)(game->posX)] == '1')
            game->posY += 0;
    }
    if (mlx_is_key_down(game->mlx, MLX_KEY_A))
    {
        if (map[(int)(game->posY)][(int)(game->posX - game->planeX * MOVE_SPEED)] == '0')
            game->posX -= game->planeX * MOVE_SPEED;
        if (map[(int)(game->posY - game->planeY * MOVE_SPEED)][(int)(game->posX)] == '0')
            game->posY -= game->planeY * MOVE_SPEED;
        if (map[(int)(game->posY)][(int)(game->posX)] == '1')
            game->posX += 0;
        if (map[(int)(game->posY)][(int)(game->posX)] == '1')
            game->posY += 0;
    }
    if (mlx_is_key_down(game->mlx, MLX_KEY_D))
    {
        if (map[(int)(game->posY)][(int)(game->posX + game->planeX * MOVE_SPEED)] == '0')
            game->posX += game->planeX * MOVE_SPEED;
        if (map[(int)(game->posY + game->planeY * MOVE_SPEED)][(int)(game->posX)] == '0')
            game->posY += game->planeY * MOVE_SPEED;
        if (map[(int)(game->posY)][(int)(game->posX)] == '1')
            game->posX += 0;
        if (map[(int)(game->posY)][(int)(game->posX)] == '1')
            game->posY += 0;
    }
    if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
    {
        double oldDirX = game->dirX;
        game->dirX = game->dirX * cos(-ROT_SPEED) - game->dirY * sin(-ROT_SPEED);
        game->dirY = oldDirX * sin(-ROT_SPEED) + game->dirY * cos(-ROT_SPEED);
        double oldPlaneX = game->planeX;
        game->planeX = game->planeX * cos(-ROT_SPEED) - game->planeY * sin(-ROT_SPEED);
        game->planeY = oldPlaneX * sin(-ROT_SPEED) + game->planeY * cos(-ROT_SPEED);
    }
    if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
    {
        double oldDirX = game->dirX;
        game->dirX = game->dirX * cos(ROT_SPEED) - game->dirY * sin(ROT_SPEED);
        game->dirY = oldDirX * sin(ROT_SPEED) + game->dirY * cos(ROT_SPEED);
        double oldPlaneX = game->planeX;
        game->planeX = game->planeX * cos(ROT_SPEED) - game->planeY * sin(ROT_SPEED);
        game->planeY = oldPlaneX * sin(ROT_SPEED) + game->planeY * cos(ROT_SPEED);
    }
    render(game);
    
}

int main(void)
{
    t_game game;

    game.mlx = mlx_init(WIDTH, HEIGHT, "Raycaster with Multiple Textures", true);
    if (!game.mlx)
        return (1);

    set_player_start(&game);

    game.textures[0] = mlx_load_png("pic/wall.png");
    game.textures[1] = mlx_load_png("pic/redbrick.png");
    game.textures[2] = mlx_load_png("pic/mossy.png");
    game.textures[3] = mlx_load_png("pic/eagle.png");
    for (int i = 1; i < 4; i++)
    {
        if (!game.textures[i])
        {
            printf("Failed to load texture %d!\n", i);
            mlx_terminate(game.mlx);
            return (1);
        }
    }

    game.img = mlx_new_image(game.mlx, WIDTH, HEIGHT);
    render(&game);

    mlx_loop_hook(game.mlx, hook, &game);
    mlx_loop(game.mlx);

    for (int i = 1; i < 4; i++)
        mlx_delete_texture(game.textures[i]);
    mlx_terminate(game.mlx);
    return (0);
}
