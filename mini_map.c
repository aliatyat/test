/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 20:59:25 by zjamaien          #+#    #+#             */
/*   Updated: 2025/07/02 20:59:26 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#define TILE_SIZE	10   // pixels per map tile
#define MINIMAP_W   (WIDTH * TILE_SIZE )
#define MINIMAP_H   (HEIGHT * TILE_SIZE)
#define OFFSET_X    (WIDTH + MINIMAP_W - 10000) // 10px from right edge
#define OFFSET_Y     1                    // 10px from top


typedef struct s_min_map
{
	int x;
	int	y;
	int px;
	int py;
	uint32_t color;
	int screen_x;
	int screen_y;
	
	
}	t_mini_map;

void	set_color(t_game *g,t_mini_map *tmm )
{
			if (g->map[tmm->y][tmm->x] == '1')
				tmm->color = 0x333333FF;
			else
				tmm->color = 0xCCCCCCFF; 
}

void draw_minimap(t_game *g)
{
	t_mini_map tmm;
	
	ft_memset(&tmm, 0, sizeof(t_mini_map));
	while (tmm.y < g->map_h)
	{
		tmm.x = 0;
		while (tmm.x < g->map_w)
		{
			set_color(g, &tmm);
			tmm.py = 0;
			while (tmm.py < TILE_SIZE)
			{
				tmm.px = 0;
				while (tmm.px < TILE_SIZE)
				{
					tmm.screen_x = OFFSET_X + tmm.x * TILE_SIZE + tmm.px;
					tmm.screen_y = OFFSET_Y + tmm.y * TILE_SIZE + tmm.py;
					mlx_put_pixel(g->img, tmm.screen_x, tmm.screen_y, tmm.color);
					tmm.px++;
				}
				tmm.py++;
			}
			tmm.x++;
		}
		tmm.y++;
	}
}

void draw_minimap_player(t_game *g)
{
	int px;
	int py;
	int	dy;
	int	dx;
	
	dy = -2;
	dx = -2;
	px = OFFSET_X + g->player.x * TILE_SIZE;
	py = OFFSET_Y + g->player.y * TILE_SIZE;
	while (dy <= 2)
	{
		while (dx <= 2)
		{
			mlx_put_pixel(g->img, px + dx, py + dy, 0xFF0000FF);
			mlx_put_pixel(g->img, px + dx + 1, py + dy + 1, 0xFF0000FF);
			mlx_put_pixel(g->img, px + dx, py + dy, 0xFF0000FF);
			mlx_put_pixel(g->img, px + dx - 1, py + dy - 1, 0xFF0000FF);
			dx++;
		}
		dy++;
	}
}

