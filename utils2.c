/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 22:29:27 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/09 17:14:49 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, s3);
	free(tmp);
	return (result);
}

char	*helper_tojoin4(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	char	*result;
	char	*q;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	s3 = ft_strtrim(s3, "\'");
	q = ft_strjoin("\"", s3);
	if (!q)
	{
		free(tmp);
		return (NULL);
	}
	result = ft_strjoin(tmp, q);
	free(tmp);
	free(q);
	if (!result)
		return (NULL);
	tmp = result;
	result = ft_strjoin(result, "\"");
	free(tmp);
	return (result);
}

char	*ft_strjoin4(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	char	*result;
	char	*q;

	if (s3[0] != '\"' && s3[ft_strlen(s3) - 1] != '\"')
		result = helper_tojoin4(s1, s2, s3);
	else
	{
		tmp = ft_strjoin(s1, s2);
		if (!tmp)
			return (NULL);
		result = ft_strjoin(tmp, s3);
		free(tmp);
	}
	return (result);
}

char	**ft_realloc_strarr(char **arr, size_t new_size)
{
	char	**new;
	size_t	i;

	new = malloc(new_size * sizeof(char *));
	if (!new)
		return (NULL);
	i = 0;
	if (arr)
	{
		while (arr[i] && i < new_size - 1)
		{
			new[i] = arr[i];
			i++;
		}
		free(arr);
	}
	while (i < new_size)
		new[i++] = NULL;
	return (new);
}
