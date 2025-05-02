/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:30 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/29 22:03:46 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_option(t_command *cmd, int *i)
{
	int	not_newline;
	int	j;

	not_newline = 1;
	while (cmd->args[*i])
	{
		j = 1;
		if (cmd->args[*i][0] == '-' && cmd->args[*i][1] == 'n')
		{
			while (cmd->args[*i][j] == 'n')
				j++;
			if (cmd->args[*i][j] == '\0')
			{
				(*i)++;
				not_newline = 0;
			}
			else
				return (not_newline);
		}
		else
			return (not_newline);
	}
	return (not_newline);
}

char	*ft_echo(t_command *cmd, t_shell *shell)
{
	int	i;
	int	not_newline;

	not_newline = 0;
	(void)shell;
	i = 1;
	not_newline = handle_option(cmd, &i);
	while (cmd->args[i])
	{
		cmd->args[i] = expand_input(cmd->args[i], shell);
		printf("%s", cmd->args[i++]);
		if (cmd->args[i])
			printf(" ");
	}
	if (not_newline == 1)
		printf("\n");
	return (NULL);
}
