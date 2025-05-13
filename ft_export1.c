/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 16:25:04 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/12 16:29:20 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_invalid(char *arg, char *equal_input, t_shell *shell)
{
	if (ft_isdigit(arg[0]))
		return (1);
	if (equal_input && *(equal_input - 1) == ' ')
	{
		return (1);
	}
	return (0);
}

int	handle_arg(char *arg, char *equal_sign, t_shell *shell)
{
	int		status;

	*equal_sign = '\0';
	shell->env = update_env_var(shell->env, arg, equal_sign + 1);
	*equal_sign = '=';
	status = (!shell->env);
	return (status);
}

int	handle_empty_arg(char *arg, t_shell *shell)
{
	shell->env = update_env_var(shell->env, arg, " ");
	return (!shell->env);
}

int	print_invalid(char *arg, int status)
{
	ft_printf("minishell: export: `%s': not a valid identifier\n", arg);
	status = 1;
	return (1);
}

int	export_entry(char *arg, t_shell *shell)
{
	char	*equal_sign;
	char	*equal_input;
	int		status;

	status = 0;
	equal_sign = ft_strchr(arg, '=');
	equal_input = ft_strchr(shell->input, '=');
	if (is_invalid(arg, equal_input, shell) == 1)
	{
		status = 1;
		//return (print_invalid(arg, status));
	}
	else  if(equal_sign)
		return (handle_arg(arg, equal_sign, shell));
	else if (!equal_sign)
		return (handle_empty_arg(arg, shell));
	return (status);
}
