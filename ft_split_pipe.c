/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:36 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/12 18:40:00 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_pipes(const char *s)
{
	int		count;
	char	quote;

	count = 1;
	quote = 0;
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
	int		index[3];
	char	*substr;
	char	*trimmed;

	ft_memset(index, 0, sizeof(index));
	index[0] = *i;
	while (s[*i])
	{
		if ((s[*i] == '\'' || s[*i] == '"') && !index[2])
			index[2] = s[*i];
		else if (s[*i] == index[2])
			index[2] = 0;
		else if (s[*i] == '|' && !index[2])
			break ;
		(*i)++;
	}
	index[1] = *i - index[0];
	while (s[*i] == '|')
		(*i)++;
	substr = ft_substr(s, index[0], index[1]);
	if (!substr)
		return (NULL);
	trimmed = ft_strtrim(substr, " \t\n");
	free(substr);
	return (trimmed);
}

int	have_herdocs(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strnstr(cmd, "<<", ft_strlen(cmd)))
		return (1);
	return (0);
}

void	prioritize_heredocs(char **cmds, int count)
{
	int		x;
	int		swapped;
	char	*swap;

	swapped = 0;
	x = 1;
	while (swapped)
	{
		swapped = 0;
		x = 1;
		while (x < count)
		{
			if (have_herdocs(cmds[x]) && !have_herdocs(cmds[x - 1]))
			{
				swap = cmds[x - 1];
				cmds[x - 1] = cmds[x];
				cmds[x] = swap;
				swapped = 1;
			}
			x++;
		}
	}
}

static int	protect_cmds(char **cmds, char *seg, int *index)
{
	if (!seg)
	{
		free_2d_array(cmds);
		return (-1);
	}
	(*index)++;
	return (0);
}

static int	check_qoute(char **cmds)
{
	int	y;

	y = 0;
	while (cmds[y])
	{
		if (has_unclosed_quotes(cmds[y]) != 0)
		{
			printf("Unclosed quotes in: %s\n", cmds[y]);
			return (-1);
		}
		y++;
	}
	return (0);
}

char	**ft_split_pipes(char *input)
{
	char	**cmds;
	int		index[3];

	ft_memset(index, 0, sizeof(index));
	if (!input)
		return (NULL);
	index[2] = count_pipes(input);
	cmds = malloc(sizeof(char *) * (index[2] + 1));
	if (!cmds)
		return (NULL);
	while (index[0] < index[2])
	{
		cmds[index[0]] = extract_segment(input, &index[1]);
		if (protect_cmds(cmds, cmds[index[0]], &index[0]) == -1)
			return (NULL);
	}
	cmds[index[0]] = NULL;
	prioritize_heredocs(cmds, index[0]);
	if (check_qoute(cmds) == -1)
		return (NULL);
	return (cmds);
}

// #include "minishell.h"

// static int	count_pipes(const char *s)
// {
// 	int		count = 1;
// 	char	quote = 0;

// 	while (*s)
// 	{
// 		if ((*s == '\'' || *s == '"') && !quote)
// 			quote = *s;
// 		else if (*s == quote)
// 			quote = 0;
// 		else if (*s == '|' && !quote)
// 			count++;
// 		s++;
// 	}
// 	return (count);
// }

// static char	*extract_segment(const char *s, int *i)
// {
// 	int		start = *i;
// 	int		len = 0;
// 	char	quote = 0;
// 	char	*substr;
//     char	*trimmed;

// 	while (s[*i])
// 	{
// 		if ((s[*i] == '\'' || s[*i] == '"') && !quote)
// 			quote = s[*i];
// 		else if (s[*i] == quote)
// 			quote = 0;
// 		else if (s[*i] == '|' && !quote)
// 			break ;
// 		(*i)++;
// 	}
// 	len = *i - start;
// 	while (s[*i] == '|')
// 		(*i)++;
// 	//return (ft_strtrim(ft_substr(s, start, len), " \t\n"));
// 	substr = ft_substr(s, start, len); // Allocate substring
//      if (!substr)
//          return (NULL);
//      trimmed = ft_strtrim(substr, " \t\n"); // Trim the substring
//      free(substr); // Free the original substring
//     return (trimmed); // Return the trimmed string
// }

// char	**ft_split_pipes(char *input)
// {
// 	char	**cmds;
// 	int		i = 0;
// 	int		pos = 0;
// 	int		n;

// 	if (!input)
// 		return (NULL);
// 	n = count_pipes(input);
// 	cmds = malloc(sizeof(char *) * (n + 1));
// 	if (!cmds)
// 		return (NULL);
// 	while (i < n)
// 	{
// 		cmds[i] = extract_segment(input, &pos);
// 		if (!cmds[i++])
// 			return (free_2d_array(cmds), NULL);
// 	}
// 	cmds[i] = NULL;
// 	return (cmds);
// }
// char	**ft_split_pipes(char *input)
// {
// 	char	**cmds;
// 	int		i;
// 	char	*token;
// 	char	*input_copy;
// 	char	*trimmed;

// 	i = 0;
// 	if (!input)
// 		return (NULL);
// 	input_copy = ft_strdup(input);
// 	if (!input_copy)
// 		return (NULL);
// 	cmds = malloc(100 * sizeof(char *));
// 	if (!cmds)
// 	{
// 		free(input_copy);
// 		return (NULL);
// 	}
// 	token = ft_strtok(input_copy, "|");
// 	while (token)
// 	{
// 		trimmed = ft_strtrim(token, " \t\n");
// 		if (!trimmed)
// 		{
// 			free_2d_array(cmds);
// 			free(input_copy);
// 			return (NULL);
// 		}
// 		cmds[i] = trimmed;
// 		token = ft_strtok(NULL, "|");
// 		i++;
// 		if (i >= 99)
// 			break ;
// 	}
// 	cmds[i] = NULL;
// 	free(input_copy);
// 	return (cmds);
// }
