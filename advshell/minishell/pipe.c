/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:28:03 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/05 20:30:05 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command *create_command(char *input, t_shell *shell)
{
	t_command *cmd;
	char **tokens;

	(void)shell;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_command));
	cmd->in_fd = STDIN_FILENO;
	cmd->out_fd = STDOUT_FILENO;
	tokens = ft_split_shell(input, ' ');
	if (!tokens)
	{
		free(cmd);
		return (NULL);
	}
	cmd->args = tokens;
	return (cmd);
}

t_command *parse_input(char *input, t_shell *shell)
{
	t_command *head;
	t_command *prev;
	char **pipe_commands;
	int i;
	t_command *cmd;
	char *expanded_input;
	char *command;
	char *pipefound;
	char **input1;
	int x;

	if (!input)
 		return (NULL);

	
	pipe_commands = NULL;
	head = NULL;
	prev = NULL;
	i = 0;
	command = input;
	
	if (!command || command[0] == '\0')
		 return (NULL);
	// pipefound = ft_strdup(input);
	// if(!pipefound)
	// {
	// 	free(pipefound);
	// 	return (NULL);
	// }
	// free(pipefound);
	// printf("SSSSSSSSSSS\n");
	// if (is_invalid_syntax(input, shell))
	// {
	// 	shell->exit_status = 2;
	// 	return (NULL);
	// }
	// input1 = split_tokens(input);
	// if(!input1)
	// {
	// 	free(pipefound);
	// 	return (NULL);
	// }
	// pipe_commands = normalize_command(input1);
	// free(input1);
	// if (!pipe_commands)
	// {
	// 	free_split(pipe_commands);
    // 	return (NULL);
	// }
	// input = join_tokens_back(pipe_commands);
	// free_split(pipe_commands);
	// if (!input)
	// {
	// 	free(pipefound);
	// 	return (NULL);
	// }
	// printf("ZAID: %s\n", input);
	if (is_invalid_syntax(input, shell))
	{
		shell->exit_status = 2;
		return (NULL);
	}
	pipe_commands = ft_split_pipes(input);
	 //free(input);
	if (!pipe_commands)
	{
		free(pipefound);
		return (NULL);
	}
	i = 0;
	while (pipe_commands[i])
	{
		printf("ALI: %s\n", pipe_commands[i]);
		i++;
	}
	// if (!expanded_input)
	// {
	// 	free(pipefound);
	// 	return (NULL);
	// }
	i = 0;
	if (!pipe_commands)
		return (NULL);
	while (pipe_commands[i])
	{
		cmd = create_command(pipe_commands[i], shell);
		x = 0;
		while (cmd->args[x])
		{
			printf("COMMAND: %s\n", cmd->args[x]);
			x++;
		}
		if (!cmd)
		{
			free_split(pipe_commands);
			free_commands(head);
			return (NULL);
		}
		if (!head)
			head = cmd;
		else
			prev->next = cmd;
		prev = cmd;
		i++;
	}
	free_split(pipe_commands);
	pipe_commands = NULL;
	return (head);
}

void setup_pipes(t_command *cmd)
{
	int pipefd[2];
	t_command *tmp;

	tmp = cmd;
	while (tmp && tmp->next)
	{
		if (pipe(pipefd) == -1)
		{
			ft_perror("pipe", 1);
			return;
		}
		if (tmp->out_fd != STDOUT_FILENO)
			close(tmp->out_fd);
		tmp->out_fd = pipefd[1];
		if (tmp->next->in_fd != STDIN_FILENO)
			close(tmp->next->in_fd);
		tmp->next->in_fd = pipefd[0];
		tmp = tmp->next;
	}
	   // Close any remaining open file descriptors
	   if (tmp && tmp->out_fd != STDOUT_FILENO)
	   close(tmp->out_fd);
  	 if (tmp && tmp->in_fd != STDIN_FILENO)
	   close(tmp->in_fd);
}
