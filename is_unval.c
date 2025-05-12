/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_unval.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:59:30 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/12 20:08:11 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_error(char *str, int i)
{
	if (*str && *str == '|')
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (2);
	}
	if (!str || i < 0)
		return (0);
	while (str[i])
	{
		if (i == 0 && str[i] == '|')
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			return (2);
		}
		if (str[i] == '|' || str[i] == '&' )
		{
			(i)++;
			while (str[i] == ' ' || str[i] == '\t')
				(i)++;
			if (!str[i] || str[i] == '|' || str[i] == '&')
			{
				printf("minishell: not supported\n");
				return (2);
			}
		}
		(i)++;
	}
	return (0);
}

int	is_invalid_syntax(char *input)
{
	int		i;
	int		in_quotes;
	char	quote_char;

	i = 0;
	in_quotes = 0;
	quote_char = 0;
	if (!input || input[0] == '\0')
		return (1);
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (input[i] && is_error(&input[i], i) != 0)
			return (2);
		else
			return (0);
		i++;
	}
	return (0);
}
