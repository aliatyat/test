/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:36 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/03 21:46:32 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 #include "minishell.h"
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
	char	*substr;
    char	*trimmed;

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
	//return (ft_strtrim(ft_substr(s, start, len), " \t\n"));
	substr = ft_substr(s, start, len); // Allocate substring
     if (!substr)
         return (NULL);
     trimmed = ft_strtrim(substr, " \t\n"); // Trim the substring
     free(substr); // Free the original substring
    return (trimmed); // Return the trimmed string
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
