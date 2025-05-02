/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:28:03 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/01 21:08:47 by alalauty         ###   ########.fr       */
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
		printf("quoted command %c\n", input[i + 3]);
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

int	is_invalid_syntax(char *input)
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
		if (!in_quotes && (input[i] == '\'' || input[i] == '"'))
		{
			in_quotes = 1;
			quote_char = input[i++];
			while (input[i] && input[i] != quote_char)
				i++;
			if (!input[i]) // Unclosed quote
				return (1);
			in_quotes = 0;
			i++;
			continue;
		}

		// Detect pipe at start
		if (i == 0 && input[i] == '|')
			return (2);

		// Detect invalid pipe use (|| or trailing |)
		if (input[i] == '|')
		{
			i++;
			while (input[i] == ' ' || input[i] == '\t')
				i++;
			if (!input[i] || input[i] == '|') // end or double pipe
				return (2);
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
		return (NULL);
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
	pipefound = ft_strdup(input);
	if(!pipefound)
	{
		free(pipefound);
		return (NULL);
	}
	free(pipefound);
	printf("SSSSSSSSSSS\n");
	if (is_invalid_syntax(input))
		return (NULL);
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
	
	expanded_input = expand_input(input, shell);
	//free(input);
	if (!expanded_input)
	{
		free(pipefound);
		return (NULL);
	}
	printf("EXP: %s\n", expanded_input);
	pipe_commands = ft_split_pipes(expanded_input);
	free(expanded_input);
	if (!pipe_commands)
	{
		free(pipefound);
		return (NULL);
	}
	i = 0;
	while (pipe_commands[i])
	{
		printf("ALI: %s\n", pipe_commands[i]);
		pipe_commands[i] = expand_input(pipe_commands[i], shell);
		if (!pipe_commands[i])
		{
			free(pipefound);
			free_split(pipe_commands);
			return (NULL);
		}
		i++;
	}
	if (!expanded_input)
	{
		free(pipefound);
		return (NULL);
	}
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
