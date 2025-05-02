/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:16 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/29 18:10:14 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Helper function to get PATH directories */
char	**get_paths_from_env(char **env)
{
	char	*path;
	char	**paths;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		return (NULL);
	path = env[i] + 5;
	paths = ft_split(path, ':');
	return (paths);
}

/* Find full path of command */
char	*find_path(char **env, char *cmd)
{
	char	**paths;
	char	*full_path;
	int		i;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	paths = get_paths_from_env(env);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin3(paths[i], "/", cmd);
		if (!full_path)
			break ;
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}

/* Close all pipe file descriptors */
void	close_all_pipes(t_command *cmd)
{
	t_command	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->in_fd != STDIN_FILENO)
		{
			close(tmp->in_fd);
			tmp->in_fd = STDIN_FILENO;
		}
		if (tmp->out_fd != STDOUT_FILENO)
		{
			close(tmp->out_fd);
			tmp->out_fd = STDOUT_FILENO;
		}
		tmp = tmp->next;
	}
}

/* Convert environment list to array */
char	**env_to_array(char **env)
{
	int		count;
	char	**array;
	int		i;

	count = 0;
	while (env[count])
		count++;
	array = malloc((count + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	while (env[i])
	{
		array[i] = ft_strdup(env[i]);
		if (!array[i])
		{
			while (i-- > 0)
				free(array[i]);
			free(array);
			return (NULL);
		}
		i++;
	}
	array[i] = NULL;
	return (array);
}
