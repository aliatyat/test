/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_in.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:19 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/05 15:07:32 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	int	i;

	char *builtins[] = {
		"cd", "echo", "pwd", "export", "unset", "env", "exit", NULL};
	i = 0;
	if (!cmd)
		return (0);
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	handle_common_builtins(char *command, t_command *cmd, t_shell *shell)
{
	if (ft_strcmp(command, "echo") == 0)
	{
		ft_echo(cmd, shell);
		return (shell->exit_status = 0);
	}
	else if (ft_strcmp(command, "pwd") == 0)
	{
		shell->exit_status = ft_pwd(cmd);
		return (shell->exit_status);
	}
	else if (ft_strcmp(command, "env") == 0)
	{
		shell->exit_status = ft_env(shell);
		return (shell->exit_status);
	}
	return (0);
}

int	handle_special_builtins(char *command, t_command *cmd, t_shell *shell)
{
	if (ft_strcmp(command, "cd") == 0)
	{
		shell->exit_status = ft_cd(cmd, shell);
		return (shell->exit_status);
	}
	else if (ft_strcmp(command, "export") == 0)
	{
		shell->exit_status = ft_export(cmd, shell);
		return (shell->exit_status);
	}
	else if (ft_strcmp(command, "unset") == 0)
	{
		shell->exit_status = ft_unset(cmd, shell);
		return (0);
	}
	else if (ft_strcmp(command, "exit") == 0)
	{
		shell->exit_status = ft_exit(cmd, shell);
		return (shell->exit_status);
	}
	return (0);
}

int	execute_builtin(t_command *cmd, t_shell *shell)
{
	char	*command;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	command = cmd->args[0];
	if (handle_common_builtins(command, cmd, shell))
		return (shell->exit_status);
	return (handle_special_builtins(command, cmd, shell));
}

int	execute_command(t_command *cmd, t_shell *shell)
{
	int	i;
	int	x;
	int	z;

	i = 0;
	x = 0;
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	while (shell->input[i])
	{
		if (shell->input[i] == '|' || shell->input[i] == '>'
			|| shell->input[i] == '<')
		{
			x = 1;
			break ;
		}
		i++;
	}
	if (cmd->next == NULL)
		if (!cmd->next && ft_strcmp(cmd->args[0], "cd") == 0)
			return (execute_builtin(cmd, shell));
	if (!cmd->next && (is_builtin(cmd->args[0]) && x == 0))
	{
		execute_builtin(cmd, shell);
		return (shell->exit_status);
	}
	close_all_pipes(cmd);
	return (execute_pipeline(cmd, shell));
}
