/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:29:10 by zjamaien          #+#    #+#             */
/*   Updated: 2025/07/03 18:32:50 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/game.h"

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
			tmm->color = 0x333333FF;
		else
			tmm->color = 0xCCCCCCFF;
	}
	else
		tmm->color = 0x000000FF;
}

#define VIEW_RADIUS 6

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

