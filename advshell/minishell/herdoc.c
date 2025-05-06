/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:44 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/05 21:35:10 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_here(char *input, t_shell *shell)
{
	char	buffer[4096];
	size_t	buf_index;
	int		in_single_quote;
	int		in_double_quote;
	char	*ptr;
	char	*exit_status;
	char	*var_start;
	char	*var_name;
	char	*value;

	buf_index = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	ptr = input;
	if (!input)
		return (NULL);
	while (*ptr && buf_index < sizeof(buffer) - 1)
	{
		if (*ptr == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			buffer[buf_index++] = *ptr++;
			continue ;
		}
		else if (*ptr == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			buffer[buf_index++] = *ptr++;
			continue ;
		}
		if (*ptr == '$' && !in_single_quote && *(ptr + 1))
		{
			ptr++;
			if (*ptr == '?')
			{
				if(g_signal_status != -1)
					exit_status = ft_itoa(g_signal_status);
				else
				exit_status = ft_itoa(shell->exit_status);
				if (exit_status)
				{
					ft_strlcpy(buffer + buf_index, exit_status, sizeof(buffer)
						- buf_index);
					buf_index += ft_strlen(exit_status);
					free(exit_status);
				}
				ptr++;
				continue ;
			}
			else if (ft_isalpha(*ptr) || *ptr == '_')
			{
				var_start = ptr;
				while (ft_isalnum(*ptr) || *ptr == '_')
					ptr++;
				var_name = ft_substr(var_start, 0, ptr - var_start);
				value = get_env_value(shell->env, var_name);
				if (value)
				{
					ft_strlcpy(buffer + buf_index, value, sizeof(buffer)
						- buf_index);
					buf_index += ft_strlen(value);
				}
				free(var_name);
				continue ;
			}
			buffer[buf_index++] = '$';
		}
		else
		{
			buffer[buf_index++] = *ptr++;
		}
	}
	buffer[buf_index] = '\0';
	return (ft_strdup(buffer));
}

int	handle_heredoc(t_command *cmd, char *delimiter, t_shell *shell)
{
	int		pipe_fd[2];
	char	*line;
	char	*new_ex;

	line = NULL;
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell");
		return (-1);
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	while (1)
	{
		if (delimiter[0] != '$' && delimiter[0] == '\"')
		{
			new_ex = ft_strdup(delimiter);	
			if (!new_ex)
            {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
                return (-1);
            }
			delimiter =  expand_input(delimiter, shell);
		}
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (line[0] == '\0')
		{
			free(line);
			break ;
		}
		if (!line || (ft_strlen(line) == ft_strlen(delimiter) + 1
				&& ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
				&& line[ft_strlen(delimiter)] == '\n'))
		{
			free(line);
			break ;
		}
		if ((delimiter[0] == '\'' && delimiter[ft_strlen(delimiter) - 1] == '\'') || 
			(new_ex[0] == '\"' && new_ex[ft_strlen(new_ex) - 1] == '\"'))
			{
				write(pipe_fd[1], line, ft_strlen(line));
			}
		else
		{
			line = expand_here(line, shell);
			write(pipe_fd[1], line, ft_strlen(line));
		}
		free(line);
	}
	close(pipe_fd[1]);
	if (cmd->in_fd != STDIN_FILENO)
		close(cmd->in_fd);
	cmd->in_fd = pipe_fd[0];
	return (0);
}
