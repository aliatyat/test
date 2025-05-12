/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 22:30:23 by zjamaien          #+#    #+#             */
/*   Updated: 2025/05/12 14:59:44 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define AC n[0]
#define R n[1]
#define START n[2]
char	**gather_back(char **tokens, char *cmd, char **args, char **redir)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (cmd)
		tokens[i++] = cmd;
	while (args[j])
	{
		tokens[i++] = args[j++];
	}
	j = 0;
	while (redir[j])
	{
		tokens[i++] = redir[j++];
	}
	tokens[i] = NULL;
	return (tokens);
}

void	null_terminate(char **redir, char **argc, int end_argc, int end_redir)
{
	redir[end_redir] = NULL;
	argc[end_argc] = NULL;
}

static char	*join_red(char *s1, char *s2, int *i)
{
	char	*temp;

	temp = ft_strjoin(s1, s2);
	if (!temp)
		return (NULL);
	(*i)++;
	return (temp);
}

int	protect_redir(char *red, char **redir, char **args)
{
	if (red == NULL)
	{
		free_2d_array(redir);
		free_2d_array(args);
		return (-1);
	}
	return (0);
}

void	fill_args_and_redir(char **tokens, char **args, char **redir,
		char **cmd)
{
	int	n[3];

	ft_memset(n, 0, sizeof(n));
	while (START < count_tokens2(tokens))
	{
		if (is_redirection(tokens[START]) && (START < count_tokens2(tokens)))
		{
			if (is_redirection_token(tokens[START]) && tokens[START + 1])
			{
				redir[R] = join_red(tokens[START], tokens[START + 1], &START);
				if (protect_redir(redir[R], redir, args) == -1)
					return ;
				R++;
			}
			else
				redir[R++] = ft_strdup(tokens[START]);
		}
		else if (!(*cmd))
			*cmd = ft_strdup(tokens[START]);
		else
			args[AC++] = ft_strdup(tokens[START]);
		START++;
	}
	null_terminate(redir, args, AC++, R++);
}

int	count_heredocs(char **tokens)
{
	int	count;
	int	xr;

	count = 0;
	xr = 0;
	while (tokens && tokens[xr])
	{
		if (ft_strncmp(tokens[xr], "<<", 2) == 0)
			count++;
		xr++;
	}
	return (count);
}

void	shift_tokens_if_first_is_lt_and_has_heredoc(char **tokens)
{
	int	xr;
	int	count;
	int	total;

	if (!tokens || !tokens[0])
		return ;
	xr = 0;
	count = count_heredocs(tokens);
	if (ft_strncmp(tokens[0], "<", 1) == 0 && ft_strncmp(tokens[0], "<<",
			2) != 0 && count >= 1)
	{
		xr = 0;
		total = count_tokens2(tokens);
		free(tokens[0]);
		while (xr < total - 1)
		{
			tokens[xr] = tokens[xr + 1];
			xr++;
		}
		tokens[xr] = NULL;
	}
}

static void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
}

char	**normalize_segment(char **tokens, int start, int ac, int end)
{
	char	**args;
	char	**redir;
	char	*cmd;

	args = malloc(sizeof(char *) * (end - start + 10));
	redir = malloc(sizeof(char *) * (end - start + 10));
	if (!args || !redir)
		return (NULL);
	cmd = NULL;
	shift_tokens_if_first_is_lt_and_has_heredoc(tokens);
	fill_args_and_redir(tokens, args, redir, &cmd);
	free_tokens(tokens);
	tokens = gather_back(tokens, cmd, args, redir);
	free(redir);
	free(args);
	if (!cmd)
		return (tokens);
	return (tokens);
}
