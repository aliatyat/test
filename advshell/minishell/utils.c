/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:28:17 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/05 20:36:59 by alalauty         ###   ########.fr       */
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

char	*ft_strjoin4(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	char	*result;
	char	*q;
	if (s3[0] != '\"' && s3[ft_strlen(s3) - 1] != '\"')
	{
		
			tmp = ft_strjoin(s1, s2);
			if (!tmp)
				return (NULL);
			s3 = ft_strtrim(s3, "\'");
			q =	ft_strjoin("\"", s3);
			if (!q)
			{
				free(tmp);
				return (NULL);
			}
			result = ft_strjoin(tmp, q);
			free(tmp);
			free (q);
			if(!result)
				return NULL;
			tmp = result;
			result = ft_strjoin(result, "\"");
			free(tmp);
	}
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

int	count_tokens(const char *str, char delim)
{
	int		count;
	int		in_quotes;
	char	quote_char;

	count = 0;
	in_quotes = 0;
	quote_char = 0;
	while (*str)
	{
		if (!in_quotes && (*str == '\'' || *str == '"'))
		{
			quote_char = *str;
			in_quotes = 1;
			str++;
			continue ;
		}
		else if (in_quotes && *str == quote_char)
		{
			in_quotes = 0;
			str++;
			continue ;
		}
		if (!in_quotes && *str == delim)
		{
			count++;
			while (*str == delim)
				str++;
			continue ;
		}
		str++;
	}
	return (count + 1);
}


char	*get_next_token(char **str, char delim)
{
	const char	*start;
	char		*token;
	int			in_quotes;
	char		quote_char;
	int			len;

	in_quotes = 0;
	quote_char = 0;
	len = 0;

	// Skip leading delimiters and whitespace
	while (**str && (**str == delim || **str == ' ' || **str == '\t'))
		(*str)++;
	start = *str;

	// Count token length while respecting quotes
	while (**str)
	{
		if (!in_quotes && (**str == '\'' || **str == '"'))
		{
			printf("inside quotes%s\n", *str);
			quote_char = **str;
			in_quotes = 1;
		}
		else if (in_quotes && **str == quote_char)
			in_quotes = 0;
		else if (!in_quotes && **str == delim)
			break;
		(*str)++;
		len++;
	}

	// Allocate and copy token
	token = malloc((*str - start) + 1);
	if (!token)
		return (NULL);
	ft_strlcpy(token, start, (*str - start) + 1);
	printf("token 2  %s\n", token);
	return (token);
}

char	**ft_split_shell(char *str, char delim)
{
	char	**result;
	int		token_count;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	token_count = count_tokens(str, delim);
	result = malloc((token_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (i < token_count)
	{
		result[i] = get_next_token(&str, delim);
		printf("tooooo 1  %s\n", result[i]);
		if (!result[i])
		{
			free_split(result);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}

int has_unclosed_quotes(char *str)
{
    int in_quotes = 0;
    char quote_char = 0;
    
    while (*str)
    {
        if (!in_quotes && (*str == '\'' || *str == '"'))
        {
            quote_char = *str;
            in_quotes = 1;
        }
        else if (in_quotes && *str == quote_char)
        {
            in_quotes = 0;
        }
        str++;
    }
    return in_quotes;
}
