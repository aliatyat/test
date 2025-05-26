// src/player.c
#include "../includes/game.h"

void init_player(t_player *player)
{
    player->x = 0;
    player->y = 0;
    player->angle = 0;
    player->key_up = false;
    player->key_down = false;
    player->key_left = false;
    player->key_right = false;
    player->left_rotate = false;
    player->right_rotate = false;
}

int key_press(int keycode, t_player *player)
{
    if (keycode == W)
        player->key_up = true;
    if (keycode == S)
        player->key_down = true;
    if (keycode == A)
        player->key_left = true;
    if (keycode == D)
        player->key_right = true;
    if (keycode == LEFT)
        player->left_rotate = true;
    if (keycode == RIGHT)
        player->right_rotate = true;
    if (keycode == ESC)
        exit(0);
    return 0;
}

int key_release(int keycode, t_player *player)
{
    if (keycode == W)
        player->key_up = false;
    if (keycode == S)
        player->key_down = false;
    if (keycode == A)
        player->key_left = false;
    if (keycode == D)
        player->key_right = false;
    if (keycode == LEFT)
        player->left_rotate = false;
    if (keycode == RIGHT)
        player->right_rotate = false;
    return 0;
}

void move_player(t_player *player, t_game *game)
{
    float speed = 3.0;
    float angle_speed = 0.05;
    float move_x = 0, move_y = 0;

    if (player->left_rotate)
        player->angle -= angle_speed;
    if (player->right_rotate)
        player->angle += angle_speed;
    if (player->angle > 2 * PI)
        player->angle -= 2 * PI;
    if (player->angle < 0)
        player->angle += 2 * PI;

    if (player->key_up)
    {
        move_x += cos(player->angle) * speed;
        move_y += sin(player->angle) * speed;
    }
    if (player->key_down)
    {
        move_x -= cos(player->angle) * speed;
        move_y -= sin(player->angle) * speed;
    }
    if (player->key_left)
    {
        move_x -= cos(player->angle + PI / 2) * speed;
        move_y -= sin(player->angle + PI / 2) * speed;
    }
    if (player->key_right)
    {
        move_x += cos(player->angle + PI / 2) * speed;
        move_y += sin(player->angle + PI / 2) * speed;
    }

    float magnitude = sqrt(move_x * move_x + move_y * move_y);
    if (magnitude > 0)
    {
        move_x = (move_x / magnitude) * speed;
        move_y = (move_y / magnitude) * speed;
    }

    float new_x = player->x + move_x;
    float new_y = player->y + move_y;
    if (!touch(new_x, new_y, game))
    {
        player->x = new_x;
        player->y = new_y;
    }
}