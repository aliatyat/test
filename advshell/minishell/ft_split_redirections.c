/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:41 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/04 19:03:42 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(char *s, size_t n)
{
	char	*new;
	size_t	len;

	len = ft_strlen(s);
	if (n < len)
		len = n;
	new = malloc(len + 1);
	if (!new)
		return (NULL);
	ft_memcpy(new, s, len);
	new[len] = '\0';
	return (new);
}

char	**split_with_redirections(char *input)
{
	char	*ptr;
	int		i;
	char	quote;
	char	*start;
	char	**tokens;

	tokens = malloc(100 * sizeof(char *));
	if (!tokens)
		return (NULL);
	ptr = input;
	i = 0;
	while (*ptr)
	{
		while (*ptr == ' ' || *ptr == '\t')
			ptr++;
		if (!*ptr)
			break ;

		// Handle redirection if not quoted
		if (*ptr == '>' || *ptr == '<')
		{
			if (ptr[1] == *ptr)
				tokens[i++] = ft_strndup(ptr, 2), ptr += 2;
			else
				tokens[i++] = ft_strndup(ptr, 1), ptr += 1;
			continue ;
		}

		// Handle normal token (possibly quoted)
		start = ptr;
		quote = 0;
		while (*ptr && (quote || (*ptr != ' ' && *ptr != '\t' && *ptr != '>' && *ptr != '<')))
		{
			if ((*ptr == '"' || *ptr == '\''))
			{
				if (quote == 0)
					quote = *ptr;
				else if (quote == *ptr)
					quote = 0;
			}
			ptr++;
		}
		tokens[i++] = ft_strndup(start, ptr - start);
	}
	tokens[i] = NULL;
	return (tokens);
}
