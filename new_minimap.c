/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:29:10 by zjamaien          #+#    #+#             */
/*   Updated: 2025/07/03 19:21:19 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/game.h"

#define VIEW_RADIUS 6
typedef struct s_min_map
{
	int			x;
	int			y;
	int			px;
	int			py;
	uint32_t	color;
	int			screen_x;
	int			screen_y;
	int			start_y;
	int			start_x;
	int			map_y;
	int			map_x;
	int			dy;
	int			dx;

}				t_mini_map;

void	set_color(t_game *g, t_mini_map *tmm)
{
	if (tmm->map_y >= 0 && tmm->map_y < g->map_h && tmm->map_x >= 0
		&& tmm->map_x < g->map_w)
	{
		if (g->map[tmm->map_y][tmm->map_x] == '1')
			tmm->color = 0x333333FF; // Wall
		else
			tmm->color = 0xCCCCCCFF; // Empty space
	}
	else
		tmm->color = 0x000000FF;
}



void	drow_player_mm(t_game *g, t_mini_map *tmm)
{
	tmm->screen_x = OFFSET_X + VIEW_RADIUS * TILE_SIZE + TILE_SIZE + 1;
	tmm->screen_y = OFFSET_Y + VIEW_RADIUS * TILE_SIZE + TILE_SIZE + 1;
	tmm->dy = -1;
	while (tmm->dy <= 1)
	{
		tmm->dx = -1;
		while (tmm->dx <= 1)
		{
			mlx_put_pixel(g->img, tmm->screen_x + tmm->dx, tmm->screen_y
					+ tmm->dy, 0xFF0000FF);
			tmm->dx++;
		}
		tmm->dy++;
	}
}

void	drow_screen_mm(t_game *g, t_mini_map *tmm)
{
	tmm->px = 0;
	while (tmm->px < TILE_SIZE)
	{
		tmm->screen_x = OFFSET_X + tmm->x * TILE_SIZE + tmm->px;
		tmm->screen_y = OFFSET_Y + tmm->y * TILE_SIZE + tmm->py;
		mlx_put_pixel(g->img, tmm->screen_x, tmm->screen_y, tmm->color);
		tmm->px++;
	}
}

void	draw_minimap(t_game *g)
{
	t_mini_map	tmm;

	ft_memset(&tmm, 0, sizeof(t_mini_map));
	tmm.start_y = g->player.y - VIEW_RADIUS;
	tmm.start_x = g->player.x - VIEW_RADIUS;
	while (tmm.y < VIEW_RADIUS * 2 + 1)
	{
		tmm.x = 0;
		while (tmm.x < VIEW_RADIUS * 2 + 1)
		{
			tmm.map_y = tmm.start_y + tmm.y;
			tmm.map_x = tmm.start_x + tmm.x;
			set_color(g, &tmm);
			tmm.py = 0;
			while (tmm.py < TILE_SIZE)
			{
				drow_screen_mm(g, &tmm);
				tmm.py++;
			}
			tmm.x++;
		}
		tmm.y++;
	}
	drow_player_mm(g, &tmm);
}
