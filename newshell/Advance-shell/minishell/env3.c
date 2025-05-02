/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:33:36 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/01 20:56:27 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**remove_env_var(char **env, char *var)
{
	int		count;
	int		i;
	int		j;
	char	**new_env;
	size_t	var_len;

	count = 0;
	i = 0;
	j = 0;
	var_len = ft_strlen(var);
	while (env[count])
		count++;
	new_env = malloc((count + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, var_len) != 0 || env[i][var_len] != '=')
		{
			new_env[j] = ft_strdup(env[i]);
			if (!new_env[j])
			{
				while (j-- > 0)
					free(new_env[j]);
				free(new_env);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	
	free_split(env);
	return (new_env);
}

void	print_sorted_env(char **env, int fd)
{
	int		count;
	char	**sorted_env;
	int		i;
	int		j;
	char	*temp;

	count = 0;
	while (env[count])
		count++;
	sorted_env = copy_env(env);
	if (!sorted_env)
		return ;
	for (i = 0; i < count - 1; i++)
	{
		for (j = 0; j < count - i - 1; j++)
		{
			if (ft_strcmp(sorted_env[j], sorted_env[j + 1]) > 0)
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
		}
	}
	i = 0;
	while (sorted_env[i])
	{
		if (ft_strchr(sorted_env[i], '='))
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putendl_fd(sorted_env[i], fd);
		}
		i++;
	}
	free_split(sorted_env);
}
