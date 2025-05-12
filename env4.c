/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:22:03 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/12 16:28:28 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env_vars(char **env)
{
	int	count;

	count = 0;
	while (env[count])
		count++;
	return (count);
}

static int	protect_new_env(char *new_env_v, char **new_env, int n)
{
	if (!new_env_v)
	{
		while (n-- > 0)
			free(new_env[n]);
		free(new_env);
		return (-1);
	}
	return (0);
}

char	**create_filtered_env(char **env, char *var, size_t var_len, int count)
{
	char	**new_env;
	int		n[2];

	new_env = malloc((count + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	ft_memset(n, 0, sizeof(n));
	while (env[n[0]])
	{
		if (ft_strncmp(env[n[0]], var, var_len) != 0
			|| env[n[0]][var_len] != '=')
		{
			new_env[n[1]] = ft_strdup(env[n[0]]);
			if (protect_new_env(new_env[n[1]], new_env, n[1]) == -1)
				return (NULL);
			n[1]++;
		}
		n[0]++;
	}
	new_env[n[1]] = NULL;
	return (new_env);
}
