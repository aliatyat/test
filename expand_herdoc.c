/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_herdoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:04:41 by zjamaien          #+#    #+#             */
/*   Updated: 2025/05/11 16:34:56 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_quotes(char c, int *in_squote, int *in_dquote)
{
	if (c == '\'' && !(*in_dquote))
	{
		*in_squote = !(*in_squote);
		return (1);
	}
	if (c == '"' && !(*in_squote))
	{
		*in_dquote = !(*in_dquote);
		return (1);
	}
	return (0);
}

static void	expand_exit_status(char *buffer, size_t *i, t_shell *shell)
{
	char	*exit_status;

	if (g_signal_status != -1)
		exit_status = ft_itoa(g_signal_status);
	else
		exit_status = ft_itoa(shell->exit_status);
	if (exit_status)
	{
		ft_strlcpy(buffer + *i, exit_status, 4096 - *i);
		*i += ft_strlen(exit_status);
		free(exit_status);
	}
}

static void	expand_variable(char **ptr, char *buffer, size_t *i, t_shell *shell)
{
	char	*var_start;
	char	*var_name;
	char	*value;
	int		len;

	var_start = *ptr;
	len = 0;
	while (ft_isalnum(var_start[len]) || var_start[len] == '_')
		len++;
	var_name = ft_substr(var_start, 0, len);
	value = get_env_value(shell->env, var_name);
	if (value)
	{
		ft_strlcpy(buffer + *i, value, 4096 - *i);
		*i += ft_strlen(value);
		free(value);
	}
	free(var_name);
	*ptr += len;
}

static void	expand_dollar(char **ptr, char *buffer, size_t *i, t_shell *shell)
{
	(*ptr)++;
	if (**ptr == '?')
	{
		expand_exit_status(buffer, i, shell);
		(*ptr)++;
	}
	else if (ft_isalpha(**ptr) || **ptr == '_')
		expand_variable(ptr, buffer, i, shell);
	else
		buffer[(*i)++] = '$';
}

static void	expand_loop(char *input, char *buffer, t_shell *shell)
{
	size_t	i;
	int		in_squote;
	int		in_dquote;
	char	*ptr;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	ptr = input;
	while (*ptr && i < 4095)
	{
		if (handle_quotes(*ptr, &in_squote, &in_dquote))
		{
			buffer[i++] = *ptr++;
			continue ;
		}
		if (*ptr == '$' && !in_squote && *(ptr + 1))
		{
			expand_dollar(&ptr, buffer, &i, shell);
			continue ;
		}
		buffer[i++] = *ptr++;
	}
	buffer[i] = '\0';
}

char	*expand_here(char *input, t_shell *shell)
{
	char	buffer[4096];

	if (!input)
		return (NULL);
	expand_loop(input, buffer, shell);
	return (ft_strdup(buffer));
}
