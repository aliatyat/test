/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_unval.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:59:30 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/05 16:27:59 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quoted_command(char *input)
{
	int	len;
	int	i;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	if ((input[i] == '\'' || input[i] == '"'))
	{
		if ((ft_strncmp(&input[i],  "<<", 2) != 0))
		{
		char	quote = input[i];
		i++;
		len = ft_strlen(input);
		if (len > 0 && input[len - 1] == quote && ft_strchr(input, ' '))
			return (1);
		}
		return (0);
	}
	return (0);
}

int	is_invalid_syntax(char *input, t_shell *shell)
{
	int	i = 0;
	int	in_quotes = 0;
	char	quote_char = 0;

	while (input[i])
	{
		// Skip spaces
		while (input[i] == ' ' || input[i] == '\t')
			i++;

		// ✅ Special case: quoted command like "<<1 cat" or "ls "
		if (is_quoted_command(input))
		return (1);
		// Handle quotes
		while(!in_quotes && (input[i] == '\'' || input[i] == '"') )
		{
				if (has_unclosed_quotes(input))
				{
				shell->exit_status = 2;
				perror("unclosed quotes NOT supported");
				return (shell->exit_status);
				}
			// if(ft_strchr(&input[i], '\"') || ft_strchr(&input[i], '\'') && ft_strchr(&input[i + 1],  '<') == 0)
			// 	return 0;
			in_quotes = 1;
			quote_char = input[i++];
			while (input[i] && input[i] != quote_char)
				i++;
			if (input[i] == '>')
			{
				printf("minishell: syntax error near unexpected token `newline'\n");
				return (1);
			}
			in_quotes = 0;
			i++;
			continue;
		}

		// Detect pipe at start
		if (i == 0 && input[i] == '|')
		{
			shell->exit_status = 2;
			printf("minishell: syntax error near unexpected token `|'\n");
			return (shell->exit_status);
		}

		// Detect invalid pipe use (|| or trailing |)
		if (input[i] == '|')
		{
			i++;
			while (input[i] == ' ' || input[i] == '\t')
				i++;
			if (!input[i] || input[i] == '|') // end or double pipe
			{
				printf("minishell: not supported `|'\n");
				return (2);
			}
		}

		// Detect invalid heredoc use
		if (input[i] == '<' && input[i + 1] == '<')
		{
			i += 2;
			while (input[i] == ' ' || input[i] == '\t')
				i++;
			if (!input[i] || input[i] == '<' || input[i] == '>' || input[i] == '|')
				return (1);
		}
		i++;
	}
	return (0);
}
