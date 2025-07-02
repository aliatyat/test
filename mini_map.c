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
