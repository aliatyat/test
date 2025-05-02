/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:09 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/01 14:27:02 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int  ft_exit(t_command *cmd, t_shell *shell)
{
    int exit_code = 0;
	
    if (cmd->args[1])
        exit_code = ft_atoi(cmd->args[1]);
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->exit_status = 1;
		return (shell->exit_status);
	}
	if (cmd->args[1])
	{
		if(ft_isalpha(*(cmd->args[1])))
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);	
			exit_code = 2;
		}
	}
    free_env(shell->env);
    free_commands(shell->commands);
    exit (exit_code);
}

int	ft_export(t_command *cmd, t_shell *shell)
{
	int		i;
	char	*equal_sign;
	char	*equal_new;
	int		status;

	i = 1;
	status = 0;
	if (!cmd->args[1])
	{
		print_sorted_env(shell->env, cmd->out_fd);
		//close(cmd->out_fd);
		return (0);
	}
	while (cmd->args[i])
	{
		equal_sign = ft_strchr(cmd->args[i], '=');
		equal_new = ft_strchr(shell->input, '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			shell->env = update_env_var(shell->env, cmd->args[i], equal_sign
					+ 1);
			printf("EQUAL:  %s\n", cmd->args[i]);
			*equal_sign = '=';
			if (!shell->env)
				status = 1;
		}
		if (!equal_new)
		{
			printf("asdasd\n");
			shell->env = update_env_var(shell->env, cmd->args[i], " ");
			if (!shell->env)
				status = 1;
		}
		if (equal_new && *(equal_new - 1) == ' ')
		{
			ft_printf("minishell: export: `%s': not a valid identifier\n",
				cmd->args[i + 1]);
			return (1);
		}
		i++;
	}
	return (status);
}

int	ft_unset(t_command *cmd, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!cmd->args[1])
		return (0);
	while (cmd->args[i])
	{
		shell->env = remove_env_var(shell->env, cmd->args[i]);
		if (!shell->env)
			status = 1;
		i++;
	}
	return (status);
}

int	ft_pwd(t_command *cmd)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		ft_putendl_fd(cwd, cmd->out_fd);
	else
		ft_perror("pwd", 1);
	return (0);
}

int	ft_env(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->commands->args[1])
	{
		printf("NO FILE %s\n", shell->env[i + 1]);
		ft_perror("No such file or directory", 127);
		return (127);
	}
	while (shell->env[i])
	{
		if (ft_strchr(shell->env[i], '='))
			ft_putendl_fd(shell->env[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}
