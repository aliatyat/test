/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:59:24 by zjamaien          #+#    #+#             */
/*   Updated: 2025/07/04 19:59:29 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/game.h"

int	check_texture_file(char *path)
{
	int	fd;

	if (!path || !*path)
		return (1);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (1);
	close(fd);
	free(path);
	return (0);
}


void	set_check_tex_ea(char *trimmed, int *tex_flags, int *i, char **texture_path)
{
	*texture_path = trimmed + 3;
	*i = 0;
	tex_flags[*i]++;
}

void	set_check_tex_we(char *trimmed, int *tex_flags, int *i, char **texture_path)
{
	*texture_path = trimmed + 3;
	*i = 1;
	tex_flags[*i]++;
}

void	set_check_tex_so(char *trimmed, int *tex_flags, int *i, char **texture_path)
{
	*texture_path = trimmed + 3;
	*i = 2;
	tex_flags[*i]++;
}

void	set_check_tex_no(char *trimmed, int *tex_flags, int *i, char **texture_path)
{
	*texture_path = trimmed + 3;
	*i = 3;
	tex_flags[*i]++;
}

int	check_duplicate_tex(int *tex_flags)
{
	int	i;

	i = 0; 
	while (i < 4)
		{
			if (tex_flags[i] > 1)
				return (1);
			i++;
		}
	return (0);
}

int	validate_texture_line(char *trimmed, int *tex_flags)
{
	char	*texture_path;
	int		i;

	texture_path = NULL;
	i = -1;
	if (ft_strncmp(trimmed, "EA ", 3) == 0)
		set_check_tex_ea(trimmed, tex_flags, &i, &texture_path);
	else if (ft_strncmp(trimmed, "WE ", 3) == 0)
		set_check_tex_we(trimmed, tex_flags, &i, &texture_path);
	else if (ft_strncmp(trimmed, "SO ", 3) == 0)
		set_check_tex_so(trimmed, tex_flags, &i, &texture_path);
	else if (ft_strncmp(trimmed, "NO ", 3) == 0)
		set_check_tex_no(trimmed, tex_flags, &i, &texture_path);
	if (texture_path && i >= 0)
	{
		if (check_duplicate_tex(tex_flags) == 1)
			return (1);
		tex_flags[i] = 1;
		texture_path = ft_strtrim(texture_path, " \n");
		if (!texture_path)
			return (1);
		if (check_texture_file(texture_path))
			return (1);
	}
	return (0);
}

int	check_map_line_for_error(char *line, int fd, int *tex_flags)
{
	char	*trem;

	trem = ft_strtrim(line, " \n");
	if (check_rubbish_data(line, trem))
	{
		free(trem);
		free(line);
		close(fd);
		clean_exit(NULL, "Error rubbish data3");
	}
	if (validate_texture_line(trem, tex_flags))
	{
		free(trem);
		free(line);
		close(fd);
		clean_exit(NULL, "Error: texture file not found or duplicates");
	}
	free(trem);
	return (0);
}

void	parse_map1(char *file, t_game *g)
{
	int		max_width;
	char	*texture_paths[4];

	ft_memset(texture_paths, 0, sizeof(char *) * 4);
	max_width = 0;
	parse_map_internal(file, g, &max_width, texture_paths);
	free_texture_paths(texture_paths);
	close(g->first_fd);
	validate_map(g);
}

void	free_close_exit(t_game *g,char *tre)
{
				free(tre);
				close(g->first_fd);
				clean_exit(g, "Error: this is the limit new line");
	
}

void	free_close_exit2(t_game *g,char *tre, int fd)
{
	free (tre);
	close (fd);
	close(g->first_fd);
	clean_exit(g, "Error rubbish data1");
	
}

void	check_six_elem(char **line, int fd, int *tex_flags)
{
	int x;

	x = 0;
	*line = get_next_line(fd);
	while (*line && x < 6)
	{
		if (*line[0] == '\n')
		{
			free(*line);
			*line = get_next_line(fd);
			continue ;
		}
		check_map_line_for_error(*line, fd, tex_flags);
		free(*line);
		*line = get_next_line(fd);
		x++;
	}
}
void	free_two_line(char *tre, char *line)
{
	if (line || *line)
		free(line);
	if (tre || *tre)
		free(tre);
}
void	loop_throgh_line(t_game *g ,char **tre, char **line, int fd)
{
	int	s;

	s = 0;
	while (*tre[0] != '1' && *tre[0] != ' ')
	{
		if (*tre[0] == '\n')
		{
			s++;
			if (s > 1000000)
				free_close_exit(g,*tre);
			free (*tre);
			*line = get_next_line(fd);
			*tre = ft_strtrim(*line, " ");
			free(*line);
			continue;
		}
		else if (*tre[0] != '1')
			free_close_exit2(g, *tre, fd);
		free_two_line(*tre, *line);
		*line = get_next_line(fd);
		*tre = ft_strtrim(*line, " ");
	}
}

int	read_map_error(char *av)
{
	int		fd;
	char	*line;
	int		tex_flags[4];
	char	*tre;

	t_game g;
	ft_memset(&g, 0, sizeof(g));
	ft_memset(&tex_flags, 0, sizeof(tex_flags));
	fd = open(av, O_RDONLY);
	g.first_fd = fd;
	check_six_elem(&line, fd, tex_flags);
	tre = ft_strtrim(line, " ");
	free(line);
	loop_throgh_line(&g , &tre, &line, fd);
	free (tre);
	parse_map1(av, &g);
	free_map_and_mlx(&g);
	while ((line = get_next_line(fd)) != NULL)
		free(line);
	close(fd);
	return (0);
}

void	free_texture_paths(char *texture_paths[4])
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (texture_paths[i])
		{
			free(texture_paths[i]);
			texture_paths[i] = NULL;
		}
		i++;
	}
}
