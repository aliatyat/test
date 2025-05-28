// utils2.c
#include "game.h"

void ft_free_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
        free(array[i++]);
    free(array);
}

void validate_map(t_game *g)
{
    printf("Player position: %.2f, %.2f\n", g->player.x, g->player.y); // DEBUG
    int player_count = 0;
    
    for (int y = 0; y < g->map_h; y++)
    {
        for (int x = 0; x < g->map_w; x++)
        {
            char c = g->map[y][x];
            
            // Check for valid characters
            if (!ft_strchr(" 01NSEW", c))
                clean_exit(g, "Invalid map character");
                
            // Count player starts
            if (ft_strchr("NSEW", c))
            {
                printf("Found player at %d,%d\n", x, y); // Before setting position
                g->player.x = x + 0.5;
                g->player.y = y + 0.5;
                printf("Set player to %.2f,%.2f\n", g->player.x, g->player.y);
                g->map[y][x] = '0'; // Replace player with floor
                player_count++;
            }
        }
    }
    
    if (player_count != 1)
        clean_exit(g, "Must have exactly one player");
        
    // Flood fill check
    // if (!is_map_closed(g))
    //     clean_exit(g, "Map not properly enclosed");
}

int is_map_closed(t_game *g)
{
    int y;
    int x;

    y = -1;
    while (++y < g->map_h)
    {
        x = -1;
        while (++x < (int)ft_strlen(g->map[y]))
        {
            if ((y == 0 || y == g->map_h - 1 || x == 0 || x == (int)ft_strlen(g->map[y]) - 1)
                && g->map[y][x] != '1' && g->map[y][x] != ' ')
                return (0);
            if (g->map[y][x] == '0' || ft_strchr("NSEW", g->map[y][x]))
            {
                if (y > 0 && g->map[y-1][x] == ' ')
                    return (0);
                if (y < g->map_h - 1 && g->map[y+1][x] == ' ')
                    return (0);
                if (x > 0 && g->map[y][x-1] == ' ')
                    return (0);
                if (x < (int)ft_strlen(g->map[y]) - 1 && g->map[y][x+1] == ' ')
                    return (0);
            }
        }
    }
    return (1);
}