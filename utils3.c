/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:38:37 by zjamaien          #+#    #+#             */
/*   Updated: 2025/05/07 19:43:50 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(char *s)
{
	return (!ft_strncmp(s, ">", 1) || !ft_strncmp(s, ">>", 2) || !ft_strncmp(s,
			"<", 1) || !ft_strncmp(s, "<<", 2));
}

int	count_tokens2(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

int	is_redirection_token(char *token)
{
	if (!token)
		return (0);
	if (ft_strlen(token) == 1)
		return (1);
	if (ft_strcmp(token, "<<") == 0 || ft_strcmp(token, ">>") == 0)
		return (1);
	return (0);
}
