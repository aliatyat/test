/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_out.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:22 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/05 19:13:39 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Main external command executor */
int	handle_child_process(t_command *cmd, t_shell *shell, char *full_path)
{
	char	**env_array;
	int i = 0;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->in_fd != STDIN_FILENO)
	{
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
		{
			ft_perror("minishell: dup2", 1);
			return (1);
		}
		close(cmd->in_fd);
	}
	if (cmd->out_fd != STDOUT_FILENO)
	{
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
		{
			ft_perror("minishell: dup2", 1);
			return (1);
		}
		close(cmd->out_fd);
	}
	close_all_pipes(cmd);
	env_array = env_to_array(shell->env);
	if (!env_array)
	{
		free(full_path);
		exit(EXIT_FAILURE);
	}
	int x =0;
	int f = 0;
	while(cmd->args[0][x])
	{
		if (cmd->args[0][x] == ' ')
			f = 1;
		
		x++;
	}
	if (f == 1)
	{
		cmd->args = ft_split(cmd->args[0], ' ');
	}
	//
	
	execve(full_path, cmd->args, env_array);
	free(full_path);
	free_split(env_array);
	ft_perror(cmd->args[0], 127);
	exit(127);
}

int	handle_parent_process(pid_t pid, t_shell *shell)
{
	int	status;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	wait(&status);
	if (WIFEXITED(status))
	{
		g_signal_status = -1;
		shell->exit_status = WEXITSTATUS(status);
		return (shell->exit_status);
	}
	if (WIFSIGNALED(status))
	{
		shell->exit_status = -1;
		g_signal_status = 128 + WTERMSIG(status);
		return (g_signal_status);
	}
	return 0;
}

int	execute_external(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	char	*full_path;
	int		x;
	char	*temp;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	cmd->args[0] = expand_input(cmd->args[0], shell);
	temp = ft_strdup(cmd->args[0]);
	x = 0;
	while (temp[x] && temp[x] != ' ')
	{
		x++;
	}
	temp[x] = '\0';
	full_path = find_path(shell->env, temp);
	free(temp);
	if (!full_path)
	{
		ft_error(cmd->args[0], "command not found", 127);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		free(full_path);
		ft_perror("fork", 1);
		return (1);
	}
	if (pid == 0)
	{
		handle_child_process(cmd, shell, full_path);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		free(full_path);
		return (handle_parent_process(pid, shell));
	}
	if (cmd->in_fd == STDIN_FILENO)
	{
		dup2(cmd->in_fd, STDIN_FILENO);
		close(cmd->in_fd);
	}
	exit(0);
}
