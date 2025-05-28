#ifndef GAME_H
# define GAME_H

# include "mlx.h"
# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdbool.h>
#include <stdio.h>
#include "../libft/libft.h"

# define WIDTH 1200
# define HEIGHT 800
# define TEX_SIZE 64
# define FOV 1.0472
# define MOVE_SPEED 0.05
# define ROT_SPEED 0.03

typedef struct s_img {
    void    *img;
    char    *addr;
    int     bpp;
    int     line_len;
    int     endian;
    int     width;
    int     height;
} t_img;

typedef struct s_player {
    double  x;
    double  y;
    double  dir_x;
    double  dir_y;
    double  plane_x;
    double  plane_y;
     int key_w, key_s, key_a, key_d;
    int key_left, key_right;
} t_player;

typedef struct s_ray {
    double  camera_x;
    double  ray_x;
    double  ray_y;
    int     map_x;
    int     map_y;
    double  side_x;
    double  side_y;
    double  delta_x;
    double  delta_y;
    double  wall_dist;
    int     step_x;
    int     step_y;
    int     side;
    int     line_h;
    int     draw_s;
    int     draw_e;
} t_ray;

typedef struct s_game {
    void        *mlx;
    void        *win;
    t_img       img;
    t_player    player;
    char        **map;
    int         map_w;
    int         map_h;
    t_img       tex[4];
    int         floor;
    int         ceiling;
} t_game;

// Main
void    init_game(t_game *g, char *map);
int     render_frame(t_game *g);
void    clean_exit(t_game *g, char *msg);

// Parsing
void    parse_map(char *file, t_game *g);
void    validate_map(t_game *g);
//void    load_texture(t_game *g, t_img *tex, char *path);
void    parse_color(char *line, int *color);

// Player
void    move_player(t_game *g);
void    rotate_player(t_game *g, double speed);

// Raycasting
void    raycast(t_game *g);
void    init_ray(t_game *g, t_ray *r, int x);
void    dda(t_game *g, t_ray *r);
void    draw_wall(t_game *g, t_ray *r, int x);

void ft_free_array(char **array);
void validate_map(t_game *g);
int is_map_closed(t_game *g);
// Add this prototype
int close_window(t_game *g);

// Ensure these match exactly:
int render_frame(t_game *g);
int key_handler(int key, t_game *g);
void my_mlx_pixel_put(t_img *img, int x, int y, int color);
void draw_floor_ceiling(t_game *g);

#endif