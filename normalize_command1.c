/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 22:30:23 by zjamaien          #+#    #+#             */
/*   Updated: 2025/05/08 21:17:44 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**gather_back(char **tokens, char *cmd, char **args, char **redir)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	tokens[i++] = cmd;
	while (args[j])
	{
		tokens[i++] = ft_strdup(args[j++]);
	}
	j = 0;
	while (redir[j])
	{
		tokens[i++] = ft_strdup(redir[j++]);
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
	free(s1);
	free(s2);
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
	if (ft_strncmp(tokens[0], "<", 1) == 0
		&& ft_strncmp(tokens[0], "<<", 2) != 0
		&& count >= 1)
	{
		xr = 0;
		total = count_tokens2(tokens);
		while (xr < total - 1)
		{
			tokens[xr] = tokens[xr + 1];
			xr++;
		}
		tokens[xr] = NULL;
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
	if (!cmd)
		return (tokens);
	tokens = gather_back(tokens, cmd, args, redir);
	//free_2d_array(args);
	//free_2d_array(redir);
	for (int i = 0; i < count_tokens2(tokens); i++)
	{
		printf("norm[%d]: %s\n", i, tokens[i]);
	}
	//free(redir);
	return (tokens);
}

// int	is_redirection(char *s)
// {
// 	return (!ft_strncmp(s, ">", 1) || !ft_strncmp(s, ">>", 2)
// 		|| !ft_strncmp(s, "<", 1) || !ft_strncmp(s, "<<", 2));
// }

// int	count_tokens2(char **tokens)
// {
// 	int	i = 0;
// 	while (tokens[i])
// 		i++;
// 	return (i);
// }

// char	**normalize_segment(char **tokens, int start, int end)
// {
// 	char	**norm = malloc(sizeof(char *) * (end - start + 10));
// 	char	**args = malloc(sizeof(char *) * (end - start + 10));
// 	char	**redir = malloc(sizeof(char *) * (end - start + 10));
// 	char	*cmd = NULL;
// 	int		i = start, r = 0, ni = 0 , a = 0;
// 	//int count = count_tokens2(tokens);
// 	while (i < end)
// 	{
// 		if (is_redirection(tokens[i]) && (i < end))
// 		{
// 			if (ft_strlen((const char *) tokens[i]) == 1 || ft_strcmp(tokens[i], "<<") == 0 || ft_strcmp(tokens[i], ">>") == 0)
// 			{
// 			 	redir[r++] = ft_strjoin(tokens[i], tokens[i+1]);
// 				 i++;
// 			}
// 			else
// 			{
// 				redir[r++] = tokens[i];
// 				//tokens[i] = NULL;
// 			}
// 		}
// 		else if (!cmd)
// 		{
// 			cmd = tokens[i];
// 			//tokens[i] = NULL;
// 		}
// 		else
// 		{
// 			args[a++] = tokens[i];
// 			//tokens[i] = NULL;
// 		}
// 		i++;
// 	}
// if (!cmd)
// {
// 	char **raw = malloc(sizeof(char *) * (end - start + 1));
// 	int x = 0;
// 	for (int i = start; i < end; i++)
// 	{
// 		raw[x++] = tokens[i];
// 		//tokens[i] = NULL;
// 	}
// 	raw[x] = NULL;
// 	free(args);
// 	free(redir);
// 	free(norm);
// 	return (raw);
// }

// 	norm[ni++] = cmd;
// 	for (i = 0; i < a; i++)
// 		norm[ni++] = args[i];
// 	for (i = 0; i < r; i++)
// 		norm[ni++] = redir[i];
// 	norm[ni] = NULL;
// 	free(args);
// 	free(redir);
// 	// free_2d_array(tokens);
// 	//free(tokens);
// 	return (norm);
// }

