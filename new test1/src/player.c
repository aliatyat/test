#include "game.h"

int key_handler(int key, t_game *g)
{
    if (key == 65307) // ESC
        clean_exit(g, "Game closed");
    if (key == 119) // W
        g->player.key_w = 1;
    if (key == 115) // S
        g->player.key_s = 1;
    if (key == 97) // A
        g->player.key_a = 1;
    if (key == 100) // D
        g->player.key_d = 1;
    if (key == 65361) // Left arrow
        g->player.key_left = 1;
    if (key == 65363) // Right arrow
        g->player.key_right = 1;
    return (0);
}

int key_release(int key, t_game *g)
{
    if (key == 119) g->player.key_w = 0;
    if (key == 115) g->player.key_s = 0;
    if (key == 97) g->player.key_a = 0;
    if (key == 100) g->player.key_d = 0;
    if (key == 65361) g->player.key_left = 0;
    if (key == 65363) g->player.key_right = 0;
    return (0);
}

void move_player(t_game *g)
{
    double move_speed = 0.05;
    double rot_speed = 0.03;
    
    // Rotation
    if (g->player.key_left)
        rotate_player(g, rot_speed);
    if (g->player.key_right)
        rotate_player(g, -rot_speed);
        
    // Movement
    if (g->player.key_w) {
        double new_x = g->player.x + g->player.dir_x * move_speed;
        double new_y = g->player.y + g->player.dir_y * move_speed;
        if (g->map[(int)new_y][(int)new_x] != '1') {
            g->player.x = new_x;
            g->player.y = new_y;
        }
    }
    // Similar for S, A, D keys...
}

// void move_player(t_game *g, double speed)
// {
//     double new_x = g->player.x + g->player.dir_x * speed;
//     double new_y = g->player.y + g->player.dir_y * speed;

//     if (g->map[(int)new_y][(int)new_x] != '1')
//     {
//         g->player.x = new_x;
//         g->player.y = new_y;
//     }
// }

void rotate_player(t_game *g, double speed)
{
    double old_dir = g->player.dir_x;
    g->player.dir_x = g->player.dir_x * cos(speed) - g->player.dir_y * sin(speed);
    g->player.dir_y = old_dir * sin(speed) + g->player.dir_y * cos(speed);

    double old_plane = g->player.plane_x;
    g->player.plane_x = g->player.plane_x * cos(speed) - g->player.plane_y * sin(speed);
    g->player.plane_y = old_plane * sin(speed) + g->player.plane_y * cos(speed);
}