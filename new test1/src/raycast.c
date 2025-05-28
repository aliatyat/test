#include "game.h"

void init_ray(t_game *g, t_ray *r, int x)
{
    r->camera_x = 2 * x / (double)WIDTH - 1; // -1 to 1 across screen
    r->ray_x = g->player.dir_x + g->player.plane_x * r->camera_x;
    r->ray_y = g->player.dir_y + g->player.plane_y * r->camera_x;
    
    // Normalize ray direction
    double inv_len = 1.0 / sqrt(r->ray_x * r->ray_x + r->ray_y * r->ray_y);
    r->ray_x *= inv_len;
    r->ray_y *= inv_len;
    
    r->map_x = (int)g->player.x;
    r->map_y = (int)g->player.y;
    r->delta_x = fabs(1 / r->ray_x);
    r->delta_y = fabs(1 / r->ray_y);
}

#include "game.h"

void draw_floor_ceiling(t_game *g)
{
    int x, y;
    
    // Draw ceiling (upper half)
    for (y = 0; y < HEIGHT / 2; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            my_mlx_pixel_put(&g->img, x, y, g->ceiling);
        }
    }
    
    // Draw floor (lower half)
    for (y = HEIGHT / 2; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            my_mlx_pixel_put(&g->img, x, y, g->floor);
        }
    }
}

void dda(t_game *g, t_ray *r)
{
    // Calculate step and initial side distances
    if (r->ray_x < 0) {
        r->step_x = -1;
        r->side_x = (g->player.x - r->map_x) * r->delta_x;
    } else {
        r->step_x = 1;
        r->side_x = (r->map_x + 1.0 - g->player.x) * r->delta_x;
    }
    if (r->ray_y < 0) {
        r->step_y = -1;
        r->side_y = (g->player.y - r->map_y) * r->delta_y;
    } else {
        r->step_y = 1;
        r->side_y = (r->map_y + 1.0 - g->player.y) * r->delta_y;
    }

    // DDA loop
    while (1) {
        if (r->side_x < r->side_y) {
            r->side_x += r->delta_x;
            r->map_x += r->step_x;
            r->side = 0;
        } else {
            r->side_y += r->delta_y;
            r->map_y += r->step_y;
            r->side = 1;
        }
        
        // Check map bounds first!
        if (r->map_x < 0 || r->map_y < 0 || 
            r->map_x >= g->map_w || r->map_y >= g->map_h)
            break;
            
        // Then check for wall hit
        if (g->map[r->map_y][r->map_x] == '1')
            break;
    }
    
    // Calculate wall distance
    if (r->side == 0)
        r->wall_dist = (r->map_x - g->player.x + (1 - r->step_x)/2) / r->ray_x;
    else
        r->wall_dist = (r->map_y - g->player.y + (1 - r->step_y)/2) / r->ray_y;
}

void draw_wall(t_game *g, t_ray *r, int x)
{
    int line_height = (int)(HEIGHT / r->wall_dist);
    int draw_start = -line_height / 2 + HEIGHT / 2;
    int draw_end = line_height / 2 + HEIGHT / 2;
    
    // Color coding:
    int color;
    if (r->side == 0) color = 0xFF0000; // Red for N/S walls
    else color = 0x0000FF;              // Blue for E/W walls
    
    // Draw the vertical stripe
    for (int y = draw_start; y < draw_end; y++) {
        my_mlx_pixel_put(&g->img, x, y, color);
    }
    
    // Debug info for center ray
    if (x == WIDTH/2) {
        printf("Center ray: Hit %d,%d (side:%d) dist:%.2f\n",
               r->map_x, r->map_y, r->side, r->wall_dist);
    }
}

void raycast(t_game *g)
{
    t_ray r;
    int x = -1;
    
    while (++x < WIDTH)
    {
        init_ray(g, &r, x);
        
        // Map position
        r.map_x = (int)g->player.x;
        r.map_y = (int)g->player.y;
        
        // Calculate delta distances
        r.delta_x = fabs(1 / r.ray_x);
        r.delta_y = fabs(1 / r.ray_y);
        
        dda(g, &r);
        draw_wall(g, &r, x);
    }
}