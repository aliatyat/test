/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:36 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/24 22:42:32 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



char	**split_tokens(char *input)
{
	
	char	**tokens = malloc(sizeof(char *) * 1024);
	int		i = 0, j = 0, k = 0;
	char	quote;

	while (input[i])
	{
		while (isspace(input[i]))
			i++;
		if (!input[i])
			break;
		 if (input[i] == '\'' || input[i] == '"')
		 {
		 	quote = input[i];
		 	j = 1;
		 	while (input[i + j] && input[i + j] != quote)
		 		j++;
		 	if (input[i + j] == quote)
				j++; // include closing quote
		 	 tokens[k++] = strndup_wrapper(&input[i], j);
		 	i += j;
		 }
		 if (!strncmp(&input[i], ">>", 2) || !strncmp(&input[i], "<<", 2))
		{
			tokens[k++] = strndup_wrapper(&input[i], 2);
			i += 2;
		}
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
		{
			tokens[k++] = strndup_wrapper(&input[i], 1);
			i++;
		}
		else
		{
			j = 0;
			while (input[i + j]
				&& !isspace(input[i + j])
				&& input[i + j] != '\'' && input[i + j] != '"'
				&& strncmp(&input[i + j], ">>", 2)
				&& strncmp(&input[i + j], "<<", 2)
				&& input[i + j] != '>' && input[i + j] != '<'
				&& input[i + j] != '|')
				j++;
			tokens[k++] = strndup_wrapper(&input[i], j);
			i += j;
		}
	}
	tokens[k] = NULL;
	return (tokens);
}

static int	count_pipes(const char *s)
{
	int		count = 1;
	char	quote = 0;

	while (*s)
	{
		if ((*s == '\'' || *s == '"') && !quote)
			quote = *s;
		else if (*s == quote)
			quote = 0;
		else if (*s == '|' && !quote)
			count++;
		s++;
	}
	return (count);
}

static char	*extract_segment(const char *s, int *i)
{
	int		start = *i;
	int		len = 0;
	char	quote = 0;

	while (s[*i])
	{
		if ((s[*i] == '\'' || s[*i] == '"') && !quote)
			quote = s[*i];
		else if (s[*i] == quote)
			quote = 0;
		else if (s[*i] == '|' && !quote)
			break ;
		(*i)++;
	}
	len = *i - start;
	while (s[*i] == '|')
		(*i)++;
	return (ft_strtrim(ft_substr(s, start, len), " \t\n"));
}

char	**ft_split_pipes(char *input)
{
	char	**cmds;
	int		i = 0;
	int		pos = 0;
	int		n;

	if (!input)
		return (NULL);
	n = count_pipes(input);
	cmds = malloc(sizeof(char *) * (n + 1));
	if (!cmds)
		return (NULL);
	while (i < n)
	{
		cmds[i] = extract_segment(input, &pos);
		if (!cmds[i++])
			return (free_2d_array(cmds), NULL);
	}
	cmds[i] = NULL;
	return (cmds);
}
