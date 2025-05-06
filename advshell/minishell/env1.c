/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:13 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/05 21:20:29 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_env(char **envp)
{
	char	**env;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	env = malloc((i + 1) * sizeof(char *));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			while (i-- > 0)
				free(env[i]);
			free(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}

char	*get_env_value(char **env, char *name)
{
	size_t	name_len;
	int		i;
	char	*xx;

	if (!env || !name)
		return (NULL);
	name_len = ft_strlen(name);
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
		{
			xx = ft_strtrim(env[i] + name_len + 1, "");
			return (xx);
		}
	}
	return (NULL);
}

char	**update_env_var(char **env, char *name, char *value)
{
	char	*new_var;
	size_t	name_len;
	int		i;

	if (!env || !name)
		return (env);
	name_len = ft_strlen(name);
	new_var = ft_strjoin4(name, "=", value);
	if (!new_var)
		return (env);
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
		{
			free(env[i]);
			env[i] = new_var;
			return (env);
		}
	}
	env = ft_realloc_strarr(env, i + 2);
	if (!env)
	{
		free(new_var);
		return (NULL);
	}
	printf("Updated env var: %s\n", new_var);
	env[i] = new_var;
	free(env[i + 1]);
	env[i + 1] = NULL;
	return (env);
}

char	**remove_null_args(char **args)
{
	int		count;
	int		i;
	char	**new_args;
	int		j;

	count = 0;
	i = 0;
	while (args[count])
		count++;
	new_args = malloc((count + 1) * sizeof(char *));
	if (!new_args)
		return (NULL);
	j = 0;
	for (i = 0; i < count; i++)
	{
		if (args[i])
		{
			new_args[j] = args[i];
			j++;
		}
	}
	new_args[j] = NULL;
	free(args);
	return (new_args);
}
