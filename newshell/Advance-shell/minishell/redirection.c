/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:28:06 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/01 20:51:09 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirection1(t_command *cmd, t_shell *shell)
{
	int		i;
	int		j;
	char	**new_args;
	char	**temp_split;

	i = 0;
	j = 0;
	new_args = NULL;
	temp_split = NULL;
	/* PHASE 1: Split merged redirections (e.g., "ls>out" -> "ls", ">",
			"out") */
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '>') || ft_strchr(cmd->args[i], '<'))
		{
			temp_split = split_with_redirections(cmd->args[i]);
			int g = 0;
		
			if (!temp_split)
			{
				ft_error("minishell", "malloc failed", 1);
				return (-1);
			}
			// Allocate space for new args (+2 for potential split tokens)
			new_args = realloc(new_args, (j + 3) * sizeof(char *));
			if (!new_args)
			{
				free_2d_array(temp_split);
				return (-1);
			}
			// Copy split tokens
			if (temp_split[0])
				new_args[j++] = temp_split[0];
			if (temp_split[1])
				new_args[j++] = temp_split[1];
			if (temp_split[2])
				new_args[j++] = temp_split[2];
			free(temp_split);
		}
		else
		{
			new_args = realloc(new_args, (j + 1) * sizeof(char *));
			if (!new_args)
				return (-1);
			new_args[j++] = ft_strdup(cmd->args[i]);
		}
		i++;
	}
	free_2d_array(cmd->args);
	new_args = realloc(new_args, (j + 1) * sizeof(char *));
	if (!new_args)
		return (-1);
	new_args[j] = NULL;
	// Replace old args with normalized ones
	cmd->args = new_args;
	/* PHASE 2: Process redirections */
	i = 0;
	while (cmd->args[i])
	{
		if (ft_strcmp(cmd->args[i], ">") == 0 && cmd->args[i + 1])
		{
			cmd->out_fd = open(cmd->args[i + 1], O_WRONLY | O_CREAT | O_TRUNC,
					0644);
			if (cmd->out_fd == -1)
			{
				ft_perror("minishell", 1);
				return (-1);
			}
			free(cmd->args[i]);
			free(cmd->args[i + 1]);
			cmd->args[i] = cmd->args[i + 1] = NULL;
			i += 2;
		}
		else if (ft_strcmp(cmd->args[i], ">>") == 0 && cmd->args[i + 1])
		{
			cmd->out_fd = open(cmd->args[i + 1], O_WRONLY | O_CREAT | O_APPEND,
					0644);
			if (cmd->out_fd == -1)
			{
				ft_perror("minishell", 1);
				return (-1);
			}
			free(cmd->args[i]);
			free(cmd->args[i + 1]);
			cmd->args[i] = cmd->args[i + 1] = NULL;
			i += 2;
		}
		else if (ft_strcmp(cmd->args[i], "<") == 0 && cmd->args[i + 1])
		{
			
			if (cmd->in_fd != STDIN_FILENO)
    			close(cmd->in_fd);
				
			cmd->in_fd = open(cmd->args[i + 1], O_RDONLY);
			
			if (cmd->in_fd == -1)
			{
				ft_perror(cmd->args[i + 1], 1);  // Now shows which file failed
				close (cmd->in_fd);
				return (-1);
			}
			//dup2(cmd->in_fd, STDIN_FILENO);
		//	int x = 0;
			//char *first = cmd->args[0];
			// if (ft_strcmp(cmd->args[0], "<") == 0)
			// {
			// 	cmd->args[0] = cmd->args[2];
			// 	cmd->args[2] = NULL;
			// }
			
			
			// else
			// {
				free(cmd->args[i]);
				free(cmd->args[i + 1]);
				cmd->args[i] = cmd->args[i + 1] = NULL;
			//}
			
			//cmd->args[i] = cmd->args[3] = NULL;
			i += 2;
			dup2(cmd->in_fd, STDIN_FILENO);
		}
		else if (ft_strcmp(cmd->args[i], "<<") == 0 && cmd->args[i + 1])
		{
			printf("HEREDOC: %s\n", cmd->args[i + 1]);
			//int tmp_fd = -1;
			
			// Handle the heredoc
			if (handle_heredoc(cmd, cmd->args[i + 1], shell) == -1) 
			{
				return (-1);
			}
			
			
		
			// // Save the current heredoc fd for potential chaining
			// tmp_fd = cmd->in_fd;
			
			// For multiple heredocs, the last one will be connected to STDIN
			if (cmd->args[i + 2] == NULL || ft_strcmp(cmd->args[i + 2], "<<") != 0) 
			{
				dup2(cmd->in_fd, STDIN_FILENO);
				close(cmd->in_fd);
			}
    
			free(cmd->args[i]);
			free(cmd->args[i + 1]);
			cmd->args[i] = cmd->args[i + 1] = NULL;
			i += 2;
		}
		else
		{
			i++;
		}
	}
	/* PHASE 3: Compact args (remove NULLs) */
	i = 0;
	j = 0;
	while (cmd->args[i])
	{
		if (cmd->args[i] != NULL)
		{
			cmd->args[j++] = cmd->args[i];
		}
		else
		{
			free(cmd->args[i]);
		}
		i++;
	}
	cmd->args[j] = NULL;
	//close (pipe_fd[1]);
	return (0);
}
int	has_redirection(char *cmd)
{
	int i = 0;

	while (cmd[i])
	{
		if (cmd[i] == '>' || cmd[i] == '<')
		{
			if (cmd[i] == '>' && cmd[i + 1] == '>')
				return (1);
			return (1);
		}
		i++;
	}

	return (0);
}