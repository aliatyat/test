#include "game.h"



static int is_map_line(char *line)
{
    if (!line || !*line)
        return 0;
    while (*line)
    {
        if (!ft_strchr(" 01NSEW", *line) && *line != '\n')
            return 0;
        line++;
    }
    return 1;
}

void parse_map(char *file, t_game *g)
{
    int fd = open(file, O_RDONLY);
    if (fd < 0)
        clean_exit(g, "Can't open map file");
    
    char *line;
    int map_started = 0;
    int max_width = 0;
    
    // First pass: count map lines and max width
    while ((line = get_next_line(fd)))
    {
        if (is_map_line(line))
        {
            map_started = 1;
            int len = ft_strlen(line);
            if (line[len-1] == '\n') len--;
            if (len > max_width) max_width = len;
            g->map_h++;
        }
        else if (map_started)
            clean_exit(g, "Invalid map format");
        free(line);
    }
    close(fd);
    
    // Allocate map memory
    g->map = ft_calloc(g->map_h + 1, sizeof(char *));
    if (!g->map)
        clean_exit(g, "Memory allocation failed");
    
    // Second pass: read map data
    fd = open(file, O_RDONLY);
    int y = 0;
    while ((line = get_next_line(fd)) && y < g->map_h)
    {
        if (is_map_line(line))
        {
            // Replace newline and pad with spaces
            char *trimmed = ft_strtrim(line, "\n");
            g->map[y] = malloc(max_width + 1);
            if (!g->map[y])
                clean_exit(g, "Memory allocation failed");
            ft_memset(g->map[y], ' ', max_width);
            ft_memcpy(g->map[y], trimmed, ft_strlen(trimmed));
            g->map[y][max_width] = '\0';
            free(trimmed);
            y++;
        }
        free(line);
    }
    g->map_w = max_width;
    close(fd);
    
    validate_map(g);
}