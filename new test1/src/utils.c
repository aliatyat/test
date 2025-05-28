#include "game.h"

void clean_exit(t_game *g, char *msg)
{
    if (msg)
        ft_putstr_fd("Error\n", 2);
        ft_putendl_fd(msg, 2);
    
    if (g)
    {
        // Free textures
        for (int i = 0; i < 4; i++)
            if (g->tex[i].img)
                mlx_destroy_image(g->mlx, g->tex[i].img);
        
        // Free map
        if (g->map)
        {
            for (int y = 0; y < g->map_h; y++)
                free(g->map[y]);
            free(g->map);
        }
        
        // Free MLX resources
        if (g->img.img)
            mlx_destroy_image(g->mlx, g->img.img);
        if (g->win)
            mlx_destroy_window(g->mlx, g->win);
        if (g->mlx)
        {
            mlx_destroy_display(g->mlx);
            free(g->mlx);
        }
    }
    exit(EXIT_FAILURE);
}

void my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
    char *dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int*)dst = color;
    // Debug test:
    if (x == WIDTH/2 && y == HEIGHT/2)
        printf("Drew pixel at center: %06X\n", color);
}

void load_texture(t_game *g, t_img *tex, char *path)
{
    printf("Loading texture: %s\n", path); // DEBUG
    char *trimmed = ft_strtrim(path, " \n");
    if (!trimmed)
    {
    printf("Failed to load texture: %s\n", path); // DEBUG
        clean_exit(g, "Memory allocation failed");
    }
    printf("Loading %s...", path);
    tex->img = mlx_xpm_file_to_image(g->mlx, trimmed, &tex->width, &tex->height);
    if (!tex->img || tex->width != TEX_SIZE || tex->height != TEX_SIZE)
    {
        free(trimmed);
        clean_exit(g, "Invalid texture");
    }
    printf("SUCCESS (%dx%d)\n", tex->width, tex->height);
    tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len, &tex->endian);
    free(trimmed);
}

void parse_color(char *line, int *color)
{
    char **rgb = ft_split(line, ',');
    if (!rgb[0] || !rgb[1] || !rgb[2])
{
    ft_free_array(rgb);
    return;
}
    
    int r = ft_atoi(rgb[0]);
    int g = ft_atoi(rgb[1]);
    int b = ft_atoi(rgb[2]);
    
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
    {
        ft_free_array(rgb);
        return;
    }
    
    *color = (r << 16) | (g << 8) | b;
    ft_free_array(rgb);
}



// int render_frame(t_game *g)
// {
//     if (!g->win) return (1);
//     raycast(g);
//     mlx_put_image_to_window(g->mlx, g->win, g->img.img, 0, 0);
//     return (0);
// }