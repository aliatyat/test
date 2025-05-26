// src/map_error.c
#include "../includes/game.h"

void free_map(char **map)
{
    if (!map)
        return;
    int i = 0;
    while (map[i])
        free(map[i++]);
    free(map);
}

static int is_map_line(char *line)
{
    if (!line || !*line || *line == '\n')
        return 0;
    while (*line && *line != '\n')
    {
        if (!ft_strchr("01NSEW ", *line))
            return 0;
        line++;
    }
    return 1;
}

static int parse_color(char *line)
{
    ft_printf("Parsing color: %s\n", line);
    char **parts = ft_split(line, ',');
    int i = 0;
    while (parts[i])
        i++;
    if (i != 3)
    {
        while (i > 0)
            free(parts[--i]);
        free(parts);
        return -1;
    }
    int r = ft_atoi(parts[0]);
    int g = ft_atoi(parts[1]);
    int b = ft_atoi(parts[2]);
    i = 0;
    while (parts[i])
        free(parts[i++]);
    free(parts);
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
        return -1;
    return (r << 16) | (g << 8) | b;
}

static void load_texture(t_game *game, t_texture *tex, char *path)
{
    char *trimmed_path = ft_strtrim(path, " \t\n");
    if (!trimmed_path || !*trimmed_path)
    {
        if (trimmed_path)
            free(trimmed_path);
        error_msg(game, INVALID_TEXTURE);
    }
    ft_printf("Loading texture: %s\n", trimmed_path);
    int fd = open(trimmed_path, O_RDONLY);
    if (fd == -1)
    {
        ft_printf("Error: Cannot open texture file %s\n", trimmed_path);
        free(trimmed_path);
        error_msg(game, INVALID_TEXTURE);
    }
    close(fd);
    tex->img = mlx_xpm_file_to_image(game->mlx, trimmed_path, &tex->width, &tex->height);
    if (!tex->img)
    {
        ft_printf("Error: Failed to load texture %s\n", trimmed_path);
        free(trimmed_path);
        error_msg(game, INVALID_TEXTURE);
    }
    tex->data = mlx_get_data_addr(tex->img, &tex->bpp, &tex->size_line, &tex->endian);
    if (!tex->data)
    {
        ft_printf("Error: Failed to get texture data for %s\n", trimmed_path);
        mlx_destroy_image(game->mlx, tex->img);
        free(trimmed_path);
        error_msg(game, INVALID_TEXTURE);
    }
    free(trimmed_path);
}

static void parse_config_line(t_game *game, char *line, int *config_count)
{
    ft_printf("Parsing config line: %s", line);
    char *trimmed = ft_strtrim(line, " \t\n");
    if (!trimmed)
        error_msg(game, MALLOC_ERR);
    if (ft_strncmp(trimmed, "NO ", 3) == 0 && trimmed[3])
        load_texture(game, &game->tex_no, trimmed + 3);
    else if (ft_strncmp(trimmed, "SO ", 3) == 0 && trimmed[3])
        load_texture(game, &game->tex_so, trimmed + 3);
    else if (ft_strncmp(trimmed, "WE ", 3) == 0 && trimmed[3])
        load_texture(game, &game->tex_we, trimmed + 3);
    else if (ft_strncmp(trimmed, "EA ", 3) == 0 && trimmed[3])
        load_texture(game, &game->tex_ea, trimmed + 3);
    else if (ft_strncmp(trimmed, "F ", 2) == 0 && trimmed[2])
        game->floor_color = parse_color(trimmed + 2);
    else if (ft_strncmp(trimmed, "C ", 2) == 0 && trimmed[2])
        game->ceiling_color = parse_color(trimmed + 2);
    else if (trimmed[0] != '\0')
    {
        ft_printf("Invalid config: %s\n", trimmed);
        free(trimmed);
        error_msg(game, INVALID_FORMAT);
    }
    if ((ft_strncmp(trimmed, "NO ", 3) == 0 && trimmed[3]) ||
        (ft_strncmp(trimmed, "SO ", 3) == 0 && trimmed[3]) ||
        (ft_strncmp(trimmed, "WE ", 3) == 0 && trimmed[3]) ||
        (ft_strncmp(trimmed, "EA ", 3) == 0 && trimmed[3]) ||
        (ft_strncmp(trimmed, "F ", 2) == 0 && trimmed[2]) ||
        (ft_strncmp(trimmed, "C ", 2) == 0 && trimmed[2]))
        (*config_count)++;
    free(trimmed);
}

static void count_map_lines(char *map_file, t_game *game, int *config_lines)
{
    int fd = open(map_file, O_RDONLY);
    if (fd == -1)
        error_msg(game, OPEN_MAP_FILE_ERR);
    char *line;
    int rows = 0;
    *config_lines = 0;
    while ((line = get_next_line(fd)))
    {
        ft_printf("Reading line: %s", line);
        if (is_map_line(line))
            rows++;
        else if (line[0] != '\n')
            (*config_lines)++;
        free(line);
    }
    if (rows == 0)
        error_msg(game, EMPTY_MAP_FILE);
    game->rows = rows;
    close(fd);
}

static void read_map(t_game *game, char *map_file, int config_lines)
{
    int fd = open(map_file, O_RDONLY);
    if (fd == -1)
        error_msg(game, OPEN_MAP_FILE_ERR);
    game->map = malloc(sizeof(char *) * (game->rows + 1));
    if (!game->map)
        error_msg(game, MALLOC_ERR);
    char *line;
    int i = 0;
    int config_count = 0;
    int max_cols = 0;
    char **temp_map = malloc(sizeof(char *) * game->rows);
    if (!temp_map)
        error_msg(game, MALLOC_ERR);
    while ((line = get_next_line(fd)))
    {
        ft_printf("Processing line: %s", line);
        if (is_map_line(line))
        {
            int len = ft_strlen(line);
            if (line[len - 1] == '\n')
                line[len - 1] = '\0';
            char *clean_line = ft_strdup(line);
            if (!clean_line)
            {
                free(line);
                error_msg(game, MALLOC_ERR);
            }
            // Remove internal spaces
            int j = 0, k = 0;
            char *no_spaces = malloc(len + 1);
            if (!no_spaces)
            {
                free(clean_line);
                free(line);
                error_msg(game, MALLOC_ERR);
            }
            while (clean_line[j])
            {
                if (clean_line[j] != ' ')
                    no_spaces[k++] = clean_line[j];
                j++;
            }
            no_spaces[k] = '\0';
            temp_map[i] = no_spaces;
            free(clean_line);
            if (ft_strlen(no_spaces) > max_cols)
                max_cols = ft_strlen(no_spaces);
            i++;
        }
        else if (line[0] != '\n' && config_count < config_lines)
        {
            parse_config_line(game, line, &config_count);
        }
        free(line);
    }
    close(fd);
    if (i != game->rows)
        error_msg(game, INVALID_FORMAT);
    i = 0;
    while (i < game->rows)
    {
        int len = ft_strlen(temp_map[i]);
        game->map[i] = malloc(max_cols + 1);
        if (!game->map[i])
            error_msg(game, MALLOC_ERR);
        ft_memset(game->map[i], ' ', max_cols);
        ft_memcpy(game->map[i], temp_map[i], len);
        game->map[i][max_cols] = '\0';
        free(temp_map[i]);
        i++;
    }
    game->map[i] = NULL;
    free(temp_map);
    ft_printf("Config count: %d, Max columns: %d\n", config_count, max_cols);
    if (config_count != 6 || game->floor_color == -1 || game->ceiling_color == -1)
        error_msg(game, INVALID_FORMAT);
    if (DEBUG)
    {
        i = 0;
        while (game->map[i])
            ft_printf("Parsed Map[%d]: %s\n", i, game->map[i++]);
    }
}

static int check_cub_extension(char *map_file)
{
    int len = ft_strlen(map_file);
    if (len < 4)
        return 0;
    if (ft_strncmp(&map_file[len - 4], ".cub", 4) != 0)
        return 0;
    return 1;
}

void get_map(char *map_file, t_game *game)
{
    if (!check_cub_extension(map_file))
        error_msg(game, INVALID_MAP_FILE);
    int config_lines;
    count_map_lines(map_file, game, &config_lines);
    game->mlx = mlx_init();
    if (!game->mlx)
        error_msg(game, MLX_INIT_ERR);
    read_map(game, map_file, config_lines);
    validate_map(game);
    game->columns = ft_strlen(game->map[0]);
    init_player(&game->player);
}