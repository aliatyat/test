#include "game.h"

// int key_handler(int key, t_game *g)
// {
//     if (key == 65307) // ESC
//         clean_exit(g, "Game closed");
//     if (key == 119) // W
//         move_player(g, MOVE_SPEED);
//     if (key == 115) // S
//         move_player(g, -MOVE_SPEED);
//     if (key == 97) // A
//         rotate_player(g, ROT_SPEED);
//     if (key == 100) // D
//         rotate_player(g, -ROT_SPEED);
//     return (0);
// }

void print_map(t_game *g)
{
    for (int y = 0; y < g->map_h; y++) {
        printf("%3d: %s\n", y, g->map[y]);
    }
}

int render_frame(t_game *g)
{
     // Handle movement
    move_player(g);
    // Clear screen
    ft_memset(g->img.addr, 0, WIDTH * HEIGHT * (g->img.bpp / 8));
    
    // Draw floor and ceiling
    draw_floor_ceiling(g);
    
    // Raycast walls
    raycast(g);
    
    // Refresh display
    mlx_put_image_to_window(g->mlx, g->win, g->img.img, 0, 0);
    return (0);
}
#include "game.h"

int close_window(t_game *g) // New wrapper function
{
    clean_exit(g, "Window closed");
    return (0);
}

int main(int ac, char **av)
{
    t_game g;

    if (ac != 2)
        clean_exit(NULL, "Usage: ./cub3D <map.cub>");
    
    ft_memset(&g, 0, sizeof(t_game));
    parse_map(av[1], &g);
    // After parse_map()
g.player.dir_x = 0;    // Facing north (negative Y)
g.player.dir_y = -1;
g.player.plane_x = 0.66;  // Field of view
g.player.plane_y = 0;
printf("Player dir: %.2f,%.2f | Plane: %.2f,%.2f\n",
       g.player.dir_x, g.player.dir_y,
       g.player.plane_x, g.player.plane_y);

  
    g.mlx = mlx_init();
    g.win = mlx_new_window(g.mlx, WIDTH, HEIGHT, "Cub3D");
    g.img.img = mlx_new_image(g.mlx, WIDTH, HEIGHT);
    if (!g.img.img) clean_exit(&g, "Image creation failed");
    g.img.addr = mlx_get_data_addr(g.img.img, &g.img.bpp, 
        &g.img.line_len, &g.img.endian);
    printf("Image buffer: %p (bpp:%d, line:%d, endian:%d)\n", 
       g.img.addr, g.img.bpp, g.img.line_len, g.img.endian);  // Should show valid address    
    // Init player direction
    g.player.dir_x = -1;
    g.player.dir_y = 0;
    g.player.plane_x = 0;
    g.player.plane_y = 0.66;
    
    mlx_loop_hook(g.mlx, render_frame, &g);
    mlx_hook(g.win, 2, 1L<<0, key_handler, &g);
    mlx_hook(g.win, 17, 0, close_window, &g); // Fixed here
    render_frame(&g); // Fixed argument passing
    mlx_loop(g.mlx);
    return (0);
}