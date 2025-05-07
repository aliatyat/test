/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 22:30:23 by zjamaien          #+#    #+#             */
/*   Updated: 2025/05/07 22:30:24 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**gather_back(char *cmd, char **args, char **redir)
{
	char	**norm;
	int		i;
	int		j;

	norm = malloc(sizeof(char *) * (count_tokens2(args) + count_tokens2(redir)
				+ 2));
	i = 0;
	j = 0;
	norm[i++] = cmd;
	while (args[j])
	{
		norm[i++] = args[j++];
	}
	j = 0;
	while (redir[j])
	{
		norm[i++] = redir[j++];
	}
	norm[i] = NULL;
	return (norm);
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
	(*i)++;
	return (temp);
}

void	fill_args_and_redir(char **tokens, char **args,
	char **redir, char **cmd)
{
	int	ac;
	int	r;
	int	start;

	start = 0;
	ac = 0;
	r = 0;
	while (start < count_tokens2(tokens))
	{
		if (is_redirection(tokens[start]) && (start < count_tokens2(tokens)))
		{
			if (is_redirection_token(tokens[start]) && tokens[start + 1])
				redir[r++] = join_red(tokens[start], tokens[start + 1], &start);
			else
				redir[r++] = tokens[start];
		}
		else if (!(*cmd))
			*cmd = tokens[start];
		else
			args[ac++] = tokens[start];
		start++;
	}
	null_terminate(redir, args, ac++, r++);
}

char	**normalize_segment(char **tokens, int start, int ac, int end)
{
	char	**norm;
	char	**args;
	char	**redir;
	char	*cmd;

	args = malloc(sizeof(char *) * (end - start + 10));
	redir = malloc(sizeof(char *) * (end - start + 10));
	norm = malloc(sizeof(char *) * (end - start + 10));
	if (!args || !redir || !norm)
		return (NULL);
	cmd = NULL;
	fill_args_and_redir(tokens, args, redir, &cmd);
	if (!cmd)
		return (tokens);
	norm = gather_back(cmd, args, redir);
	for (int i = 0; i < count_tokens2(norm); i++)
	{
		printf("norm[%d]: %s\n", i, norm[i]);
	}
	return (norm);
}

