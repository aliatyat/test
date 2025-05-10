/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 22:29:27 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/10 19:18:14 by zjamaien         ###   ########.fr       */
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

static void	ft_free(char **s1, char **s2, char **s3)
{
	if (*s1)
		free(*s1);
	if (*s2)
		free(*s2);
	if (*s3)
		free(*s3);
}

static char	*handle_qoute(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*result;
	char	*q;
	char	*x;
	char	*t;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	t = s3;
	t = ft_strtrim(s3, "\'");
	x = q;
	x = ft_strjoin("\"", t);
	if (!x)
		return (NULL);
	result = ft_strjoin(tmp, x);
	if (!result)
		return (NULL);
	free(tmp);
	tmp = result;
	result = ft_strjoin(result, "\"");
	ft_free(&tmp, &x, &t);
	return (result);
}

char	*ft_strjoin4(char *s1, char *s2, char *s3)
{
	char	*result;

	if (s3[0] != '\"' && s3[ft_strlen(s3) - 1] != '\"')
		result = handle_qoute(s1, s2, s3);
	else
		result = ft_strjoin3(s1, s2, s3);
	return (result);
}

char	**ft_realloc_strarr(char **arr, size_t new_size)
{
	char **new;
	size_t i;

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
