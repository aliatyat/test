/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:28:06 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/05 21:23:55 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
static int	is_redirection(char *s)
{
	return (!ft_strncmp(s, ">", 1) || !ft_strncmp(s, ">>", 2)
		|| !ft_strncmp(s, "<", 2) || !ft_strncmp(s, "<<", 2));
}

// static int	is_pipe(char *s)
// {
// 	return (!ft_strncmp(s, "|", 1));
// }
static int	count_tokens(char **tokens)
{
	int	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

static char	**normalize_segment(char **tokens, int start, int end)
{
	char	**norm = malloc(sizeof(char *) * (end - start + 10));
	char	**args = malloc(sizeof(char *) * (end - start + 10));
	char	**redir = malloc(sizeof(char *) * (end - start + 10));
	char	*cmd = NULL;
	int		i = start, r = 0, ni = 0 , a = 0;
	for (int j = 0; tokens[j]; j++)
	{
				printf("Token Red1 %d: %s\n", j, tokens[j]);
	}
	while (i < end)
	{
		if (is_redirection(tokens[i]) && (i < end))
		{
			
			if (ft_strlen((const char *) tokens[i]) == 1)
			{
			 	redir[r++] = ft_strjoin(tokens[i], tokens[i+1]);
				i++;
			 }
			 else
				redir[r++] = tokens[i];
			
		}
		else if (!cmd)
		{
			cmd = tokens[i];
		}
		else
		{
			args[a++] = tokens[i];
		}
		i++;
	}
if (!cmd)
{
	char **raw = malloc(sizeof(char *) * (end - start + 1));
	int x = 0;
	for (int i = start; i < end; i++)
		raw[x++] = tokens[i];
	raw[x] = NULL;
	free(args);
	free(redir);
	free(norm);
	return (raw);
}

	norm[ni++] = cmd;
	for (i = 0; i < a; i++)
		norm[ni++] = args[i];
	for (i = 0; i < r; i++)
		norm[ni++] = redir[i];
	norm[ni] = NULL;
	free(args);
	free(redir);
	for (int j = 0; norm[j]; j++)
	{
				printf("Token norm %d: %s\n", j, norm[j]);
	}
	return (norm);
}

int	handle_redirection1(t_command *cmd, t_shell *shell)
{
	cmd->args = normalize_segment(cmd->args, 0, count_tokens(cmd->args));
	for (int j = 0; cmd->args[j]; j++)
	{
				printf("Token Red2 %d: %s\n", j, cmd->args[j]);
	}
	int x = 0;
	int		i;
	int		j;
	char	**new_args;
	char	**temp_split;

	i = 0;
	j = 0;
	new_args = NULL;
	temp_split = NULL;
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '>') || ft_strchr(cmd->args[i], '<'))
		{
			temp_split = split_with_redirections(cmd->args[i]);
			
			int g = 0;
		
			if (!temp_split)
			{
				ft_error("minishell", "malloc failed", 1);
				free_2d_array(new_args);
				return (-1);
			}
			// Allocate space for new args (+2 for potential split tokens)
			new_args = realloc(new_args, (j + 3) * sizeof(char *));
			if (!new_args)
			{
				free_2d_array(temp_split);
				free_2d_array(new_args);
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
			{
				free_2d_array(new_args);
				return (-1);
			}
			new_args[j++] = ft_strdup(cmd->args[i]);
			if (!new_args[j - 1]) // Check if ft_strdup failed
            {
                free_2d_array(new_args); // Free allocated memory
                return (-1);
            }
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
			x = dup(cmd->in_fd);
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
			if (handle_heredoc(cmd, cmd->args[i + 1], shell) == -1) 
			{
				return (-1);
			}
			if (cmd->args[i + 2] == NULL || ft_strcmp(cmd->args[i + 2], "<<") != 0) 
			{
				x = dup(cmd->in_fd);
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
	close(x);
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