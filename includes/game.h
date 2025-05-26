#ifndef GAME_H
#define GAME_H

# define WIDTH 1200
# define HEIGHT 700
# define BLOCK 64
# define DEBUG 0

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363
# define ESC 65307
 

# define PI 3.14159265359

#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
# include "../libft/libft.h"

# define MALLOC_ERR "malloc() failed"
# define MLX_INIT_ERR "Failed to initialize mlx"
# define MLX_NEW_WINDOW_ERR "Failed to open a new window"
# define INVALID_NBR__ARGS "Invalid number of arguments"
# define NULL_MAP "NULL map arguments"
# define INVALID_ENTITY "Invalid entity on map's file"
# define INVALID_FORMAT "Invalid map"
# define MAP_NOT_CLOSED "Map is not closed by walls"
# define INVALID_NBR_EXIT "Invalid number of exit (E)"
# define NO_COLLECTIBLES "Map Dose not have fish (C)"
# define INVALID_NBR_PLAYERS "Ivalid number of start pos (P)"
# define UNACHIEVABLE_ENITES "Map has unachievable entities"
# define INVALID_MAP_FILE "Invalid map file extension"
# define OPEN_MAP_FILE_ERR "Failed open map file"
# define EMPITY_MAP_FILE "Mapo file is empty"
# define WALL_XPM_ERR "Failed to open wall image"
# define FLOOR_XPM_ERR "failed to open floor image"
# define COLLECTIBLE_XPM_ERR "Failed to open collectible image"
# define EXIT_XPM_ERR "Failed to open exit image"
# define PLAYER_XPM_ERR "Falied to open player image"
# define VALID_ENTITIES "ECPP01"


typedef struct s_player
{
    float x;
    float y;
    float angle;

    bool key_up;
    bool key_down;
    bool key_left;
    bool key_right;

    bool left_rotate;
    bool right_rotate;
}   t_player;

typedef struct s_game
{
    void *mlx;
    void *win;
    void *img;

    char *data;
    int bpp;
    int size_line;
    int endian;
   //char    **map;
    t_player player;
    //t_map   map;
    char **map;
    int		rows;
	int		columns;
} t_game;

void init_player(t_player *player);
int key_release(int keycode, t_player *player);
int key_press(int keycode, t_player *player);
// void move_player(t_player *player);
void move_player(t_player *player, t_game *game);
#endif