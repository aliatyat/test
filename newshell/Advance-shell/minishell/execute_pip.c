/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pip.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:18:13 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/01 20:30:30 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	start_pipe(t_command *current, t_command *next, int *pipefd)
{
	if (next == NULL)
		return (0);
	if (pipe(pipefd) == -1)
	{
		ft_perror("pipe", 1);
		return (1);
	}
	current->out_fd = pipefd[1];
	next->in_fd = pipefd[0];
	return (0);
}

void	execute_child(t_command *cmd, int prev_pipe_in, t_shell *shell)
{
	int	f;

	f = 0;
	while (cmd->args[f])
	{
		printf("CHILD: %s\n", cmd->args[f]);
		f++;
	}
	// int i = 0;
	// 	while (cmd->args[i])
	// 	{
	// 		printf("CHILD:yyyyyyyyyyyyy %s\n", cmd->args[i]);
	// 		i++;
	// 	}
	// int x = 0;
	// f = 0;
	// int z = 0;
	// while (cmd->args[x])
	// {
	// 	int inquote = 0;
	// 	z = 0;
	// 	while (cmd->args[x][z])
	// 	{
	// 		if (cmd->args[x][z] == '\"' || cmd->args[x][z] == '\'')
	// 		{
	// 			z++;
	// 			while (cmd->args[x][z] && cmd->args[x][z] != cmd->args[x][z - 1])
	// 				z++;
	// 		}
	// 		else
	// 			z++;
	// 	}	
	// 	if (cmd->args[x][z] == '<')
	// 	{
	// 		f = 1;
	// 	}
	// 	{
	// 		f = 1;
	// 	}
	// 	x++;
	// }

	//if (f == 1)
	//{
		if (handle_redirection1(cmd, shell) == -1)
			exit(127);
	//}
	 int x = 0;
	while (cmd->args[x])
	{
		cmd->args[x] = ft_strtrim(cmd->args[x], "\'\"");
		printf("CHILD:gggggggggggg %s\n", cmd->args[x]);
		x++;
	}
	if (prev_pipe_in != -1)
	{
		dup2(prev_pipe_in, STDIN_FILENO);
		//close(prev_pipe_in);
	}
	if (cmd->out_fd != STDOUT_FILENO)
	{
		dup2(cmd->out_fd, STDOUT_FILENO);
		//close(cmd->out_fd);
	}
	close_all_pipes(shell->commands);
	if (is_builtin(cmd->args[0]))
	exit(execute_builtin(cmd, shell));
	else
	exit(execute_external(cmd, shell));
}

void	parent_cleanup(int *prev_pipe_in, t_command *cmd)
{
	//(void)prev_pipe_in;
	if (*prev_pipe_in != -1)
		close(*prev_pipe_in);
	if (cmd->out_fd != STDOUT_FILENO)
		close(cmd->out_fd);
}

void	wait_for_children(t_shell *shell)
{
	int	status;

	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
	}
}

int	execute_pipeline(t_command *cmd, t_shell *shell)
{
	int			pipefd[2];
	pid_t		pid;
	t_command	*current;
	t_command	*next;
	int			prev_pipe_in;
	int			i;
	int			x;
	int			f;
	int			j;

	i = 0;
	current = cmd;
	prev_pipe_in = -1;
	while (current)
	{
		next = current->next;
		x = 0;
		f = 0;
		while (current->args[x])
		{
			if (current->args[x] && ft_strncmp(current->args[x], "<<", 2) == 0)
			{
				f = 1;
			}
			x++;
		}
		if (start_pipe(current, next, pipefd))
			return (1);
		pid = fork();
		if (pid < 0)
			return (ft_perror("fork", 1), 1);
		if (pid == 0)
			execute_child(current, prev_pipe_in, shell);
		else if (pid != 0)
		{
			j = 0;
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			if (f)
			{
				close(prev_pipe_in);
				wait_for_children(shell);
			}
			parent_cleanup(&prev_pipe_in, current);
		}
		if (next != NULL)
			prev_pipe_in = pipefd[0];
		else
			prev_pipe_in = -1;
		current = next;
	}
	wait_for_children(shell);
	return (shell->exit_status);
}
