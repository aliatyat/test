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
    int y = 0;
    int x = 0;
    printf("Player position: %.2f, %.2f\n", g->player.x, g->player.y); // DEBUG
    int player_count = 0;
    
    //  if (g->map[y][x] == 'N') 
    //  { 
    //     g->player.dir_x = -1; 
    //     g->player.dir_y = 0; 
    //     g->player.plane_x = 0; 
    //     g->player.plane_y = 0.66; 
    // } 
    // else if (g->map[y][x] == 'S') 
    // { 
    //     g->player.dir_x = 1; 
    //     g->player.dir_y = 0; 
    //     g->player.plane_x = 0; 
    //     g->player.plane_y = -0.66; 
    // } 
    // else if (g->map[y][x] == 'E') 
    // { 
    //     g->player.dir_x = 0; 
    //     g->player.dir_y = -1; 
    //     g->player.plane_x = -0.66; 
    //     g->player.plane_y = 0; 
    // } 
    // else if (g->map[y][x] == 'W') 
    // {
    //     g->player.dir_x = 0; 
    //     g->player.dir_y = 1; 
    //     g->player.plane_x = 0.66; 
    //     g->player.plane_y = 0; 
    // } 
    //g->map[y][x] = '0'; 
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
                if (g->map[y][x] == 'N') 
                { 
                    g->player.dir_x = -1; 
                    g->player.dir_y = 0; 
                    g->player.plane_x = 0; 
                    g->player.plane_y = 0.66; 
                } 
                else if (g->map[y][x] == 'S') 
                { 
                    g->player.dir_x = 1; 
                    g->player.dir_y = 0; 
                    g->player.plane_x = 0; 
                    g->player.plane_y = -0.66; 
                } 
                else if (g->map[y][x] == 'E') 
                { 
                    g->player.dir_x = 0; 
                    g->player.dir_y = -1; 
                    g->player.plane_x = -0.66; 
                    g->player.plane_y = 0; 
                } 
                else if (g->map[y][x] == 'W') 
                {
                    g->player.dir_x = 0; 
                    g->player.dir_y = 1; 
                    g->player.plane_x = 0.66; 
                    g->player.plane_y = 0; 
                } 
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

int	skip_spaces(char *line, int i)
{
	while (line[i] == ' ')
		i++;
	return (i);
}

void load_texture(t_game *g, char *line, int direction_len, int index)
{
	char	*texture_path;
	int		i;

	i = skip_spaces(line, direction_len);
	texture_path = ft_strdup(&line[i]);
	if (!texture_path)
	{
		perror("Failed to allocate memory for texture path");
		exit(EXIT_FAILURE);
	}
	g->tex[index].img = mlx_xpm_file_to_image(g->mlx,
			texture_path, &g->tex[index].width,
			&g->tex[index].height);
	if (!g->tex[index].img)
	{
		ft_putstr_fd("Failed to load texture: ", 2);
		ft_putstr_fd(texture_path, 2);
		ft_putstr_fd("\n", 2);
		free(texture_path);
		exit(EXIT_FAILURE);
	}
	g->tex[index].addr = mlx_get_data_addr(g->tex[index].img,
			&g->tex[index].bpp,
			&g->tex[index].line_len,
			&g->tex[index].endian);
	free(texture_path);
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

    if (r->map_x < 0 || r->map_y < 0 || 
        r->map_x >= g->map_w || r->map_y >= g->map_h)
        break;

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
    double move_speed = MOVE_SPEED;
    double rot_speed = ROT_SPEED;

    // Rotation
    if (g->player.key_left)
        rotate_player(g, rot_speed); // fixed: left is negative
    if (g->player.key_right)
        rotate_player(g, -rot_speed);  // fixed: right is positive

    // Move forward (W)
    if (g->player.key_w)
    {
        double new_x = g->player.x + g->player.dir_x * move_speed;
        double new_y = g->player.y + g->player.dir_y * move_speed;
    if ((int)new_y >= 0 && (int)new_y < g->map_h && (int)new_x >= 0 && (int)new_x < g->map_w && g->map[(int)new_y][(int)new_x] != '1')
    {
        g->player.x = new_x;
        g->player.y = new_y;
    }
    }
    // Move backward (S)
    if (g->player.key_s)
    {
        double new_x = g->player.x - g->player.dir_x * move_speed;
        double new_y = g->player.y - g->player.dir_y * move_speed;
        if ((int)new_y >= 0 && (int)new_y < g->map_h && (int)new_x >= 0 && (int)new_x < g->map_w && g->map[(int)new_y][(int)new_x] != '1')
        {
            g->player.x = new_x;
            g->player.y = new_y;
        }
    }
    // Strafe left (A)
    if (g->player.key_a)
    {
        double new_x = g->player.x - g->player.plane_x * move_speed;
        double new_y = g->player.y - g->player.plane_y * move_speed;
        if ((int)new_y >= 0 && (int)new_y < g->map_h && (int)new_x >= 0 && (int)new_x < g->map_w && g->map[(int)new_y][(int)new_x] != '1')
        {
            g->player.x = new_x;
            g->player.y = new_y;
        }
    }
    // Strafe right (D)
    if (g->player.key_d)
    {
        double new_x = g->player.x + g->player.plane_x * move_speed;
        double new_y = g->player.y + g->player.plane_y * move_speed;
        if ((int)new_y >= 0 && (int)new_y < g->map_h && (int)new_x >= 0 && (int)new_x < g->map_w && g->map[(int)new_y][(int)new_x] != '1')
        {
            g->player.x = new_x;
            g->player.y = new_y;
        }
    }
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
   // ft_memset(g->img.addr, 0, WIDTH * HEIGHT * (g->img.bpp / 8));
    
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
    mlx_hook(g.win, 3, 1L<<1, key_release, &g); 
    mlx_hook(g.win, 17, 0, close_window, &g); // Fixed here
    render_frame(&g); // Fixed argument passing
    mlx_loop(g.mlx);
    return (0);
}