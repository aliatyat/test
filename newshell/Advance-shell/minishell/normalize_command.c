#include "minishell.h"
#include <ctype.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>

static char	*strndup_wrapper(char *s, int n)
{
	char	*ret;

	ret = malloc(n + 1);
	if (!ret)
		return (NULL);
	ft_strncpy(ret, s, n);
	ret[n] = '\0';
	return (ret);
}

char	**split_tokens(char *input)
{
	char	**tokens;
	int		i;
	char	quote;
	int		j;
	int		k;

	tokens = malloc(sizeof(char *) * 1024);
	i = 0;
	j = 0;
	k = 0;
	while (input[i])
	{
		while (isspace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (!ft_strncmp(&input[i], ">>", 2) || !ft_strncmp(&input[i], "<<", 2))
		{
			tokens[k++] = strndup_wrapper(&input[i], 2);
			i += 2;
		}
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
		{
			tokens[k++] = strndup_wrapper(&input[i], 1);
			i++;
		}
		else
		{
			j = 0;
			while (input[i + j] && !isspace(input[i + j]) && strncmp(&input[i
					+ j], ">>", 2) && ft_strncmp(&input[i + j], "<<", 2) && input[i
				+ j] != '>' && input[i + j] != '<' && input[i + j] != '|')
				j++;
			tokens[k++] = strndup_wrapper(&input[i], j);
			i += j;
		}
	}
	tokens[k] = NULL;
	return (tokens);
}
static int	is_redirection(char *s)
{
	return (!ft_strcmp(s, ">") || !ft_strcmp(s, ">>") || !ft_strcmp(s, "<") || !ft_strcmp(s,
			"<<"));
}

static int	is_pipe(char *s)
{
	return (!ft_strcmp(s, "|"));
}

static int	count_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

static char	**normalize_segment(char **tokens, int start, int end)
{
	char	**norm;
	char	**args;
	char	**redir;
	char	*cmd;
	int		i;
	char	**raw;
	int		x;
	int		a;
	int		r;
	int		ni;

	norm = malloc(sizeof(char *) * (end - start + 10));
	args = malloc(sizeof(char *) * (end - start + 10));
	redir = malloc(sizeof(char *) * (end - start + 10));
	cmd = NULL;
	i = start;
	a = 0;
	r = 0;
	ni = 0;
	while (i < end)
	{
		if (is_redirection(tokens[i]) && (i + 1 < end))
		{
			redir[r++] = tokens[i++];
			redir[r++] = tokens[i];
		}
		else if (!cmd && !is_pipe(tokens[i]))
			cmd = tokens[i];
		else if (!is_pipe(tokens[i]))
			args[a++] = tokens[i];
		i++;
	}
	if (!cmd)
	{
		raw = malloc(sizeof(char *) * (end - start + 1));
		x = 0;
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
	return (norm);
}

char	**normalize_command(char **tokens)
{
	int		z;
	int		total;
	char	**result;
	int		i;
	char	**segment;
	int		j;
	int		start;
	int		ri;

	z = 0;
	while (tokens[z])
	{
		printf("NORMAL: %s\n", tokens[z]);
		z++;
	}
	total = count_tokens(tokens);
	result = malloc(sizeof(char *) * (total + 10));
	i = 0;
	start = 0;
	ri = 0;
	while (i <= total)
	{
		if (!tokens[i] || is_pipe(tokens[i]))
		{
			segment = normalize_segment(tokens, start, i);
			j = 0;
			while (segment[j])
				result[ri++] = segment[j++];
			free(segment);
			if (tokens[i])
				result[ri++] = tokens[i];
			start = i + 1;
		}
		i++;
	}
	result[ri] = NULL;
	return (result);
}

static int	calc_total_len(char **tokens)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (tokens[i])
		len += ft_strlen(tokens[i++]) + 1;
	return (len);
}

char	*join_tokens_back(char **tokens)
{
	int		f;
	int		total_len;
	char	*joined;
	int		i;
	int		pos;
	int		len;

	f = 0;
	while (tokens[f])
	{
		printf("JOINING BACK %s\n", tokens[f]);
		f++;
	}
	total_len = calc_total_len(tokens);
	joined = malloc(total_len);
	i = 0;
	pos = 0;
	if (!joined)
		return (NULL);
	while (tokens[i])
	{
		len = ft_strlen(tokens[i]);
		ft_memcpy(joined + pos, tokens[i], len);
		pos += len;
		if (((tokens[i + 1]) && (ft_strcmp(tokens[i], "<<") != 0))
			&& (ft_strcmp(tokens[i], ">") != 0) && (ft_strcmp(tokens[i],
					"<") != 0) && (ft_strcmp(tokens[i], ">>") != 0)&& (ft_strcmp(tokens[i], "\"") != 0))
			joined[pos++] = ' ';
		i++;
	}
	joined[pos] = '\0';
	return (joined);
}
