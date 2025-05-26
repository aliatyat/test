// includes/game.h
#ifndef GAME_H
#define GAME_H

#include <mlx.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include "../libft/libft.h"

#define WIDTH 1200
#define HEIGHT 700
#define BLOCK 64
#define DEBUG 0
#define PI 3.14159265359

#define W 119
#define A 97
#define S 115
#define D 100
#define LEFT 65361
#define RIGHT 65363
#define ESC 65307

#define MALLOC_ERR "malloc() failed"
#define MLX_INIT_ERR "Failed to initialize mlx"
#define MLX_NEW_WINDOW_ERR "Failed to open a new window"
#define INVALID_NBR_ARGS "Invalid number of arguments"
#define INVALID_MAP_FILE "Invalid map file extension"
#define OPEN_MAP_FILE_ERR "Failed to open map file"
#define EMPTY_MAP_FILE "Map file is empty"
#define INVALID_FORMAT "Invalid map format"
#define MAP_NOT_CLOSED "Map is not closed by walls"
#define INVALID_ENTITY "Invalid entity in map"
#define INVALID_TEXTURE "Invalid or missing texture"
#define INVALID_COLOR "Invalid or missing floor/ceiling color"
#define INVALID_PLAYER "Invalid number of player positions"

typedef struct s_texture
{
    void *img;
    char *data;
    int width, height;
    int bpp, size_line, endian;
} t_texture;

typedef struct s_player
{
    float x, y;
    float angle;
    bool key_up, key_down, key_left, key_right;
    bool left_rotate, right_rotate;
} t_player;

typedef struct s_game
{
    void *mlx;
    void *win;
    void *img;
    char *data;
    int bpp, size_line, endian;
    t_player player;
    char **map;
    int rows, columns;
    t_texture tex_no, tex_so, tex_we, tex_ea;
    int floor_color, ceiling_color;
} t_game;

// main.c
void put_pixel(int x, int y, int color, t_game *game);
void clear_image(t_game *game);
void draw_map(t_game *game);
float distance(float x, float y);
float fixed_dist(float x1, float y1, float x2, float y2, t_game *game);
bool touch(float px, float py, t_game *game);
void init_game(t_game *game, char *map_file);
void draw_line(t_player *player, t_game *game, float start_x, int i);
int draw_loop(t_game *game);
int quit_game(t_game *game);
void error_msg(t_game *game, char *error_msg);

// player.c
void init_player(t_player *player);
int key_press(int keycode, t_player *player);
int key_release(int keycode, t_player *player);
void move_player(t_player *player, t_game *game);

// map_error.c
void get_map(char *map_file, t_game *game);
void free_map(char **map);

// map_error2.c
void validate_map(t_game *game);

#endif