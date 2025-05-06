#include "minishell.h"

static int	is_special(char *s)
{
	return (!ft_strncmp(s, ">>", 2) || !ft_strncmp(s, "<<", 2)
		|| *s == '>' || *s == '<' || *s == '|');
}

char	**split_tokens(char *input)
{
	char	**tokens = malloc(sizeof(char *) * 1024);
	int		i = 0, j, k = 0;
	int		len;
	char	quote_char = 0;

	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (!input[i])
			break;
		if (input[i] == '\'' || input[i] == '"')
		{
			quote_char = input[i];
			j = 0;
            i++;
			while (input[i + j] && input[i + j] != quote_char)
				j++;
            if (input[i + j] == quote_char)
                j++;
			tokens[k] = ft_strndup(&input[i], j);
			if (quote_char == '"')
            	tokens[k] = ft_strjoin("\"",tokens[k]);
			else if ( quote_char == '\'')
				tokens[k] = ft_strjoin("\'",tokens[k]);
			i += j;
            k++;
           
		}
		else if (!ft_strncmp(&input[i], ">>", 2) || !ft_strncmp(&input[i], "<<", 2))
		{
			tokens[k++] = ft_strndup(&input[i], 2);
			i += 2;
		}
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
		{
			tokens[k++] = ft_strndup(&input[i], 1);
			i++;
		}
		else
		{
			j = 0;
			while (input[i + j] && !ft_isspace(input[i + j])
				&& ft_strncmp(&input[i + j], ">>", 2)
				&& ft_strncmp(&input[i + j], "<<", 2)
				&& input[i + j] != '>' && input[i + j] != '<' && input[i + j] != '|')
				j++;
			tokens[k++] = ft_strndup(&input[i], j);
			i += j;
		}
	}
	tokens[k] = NULL;
	return tokens;
}


static int	is_redirection(char *s)
{
	return (!ft_strcmp(s, ">") || !ft_strcmp(s, ">>")
		|| !ft_strcmp(s, "<") || !ft_strcmp(s, "<<"));
}

static int	is_pipe(char *s)
{
	return (!ft_strcmp(s, "|"));
}

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
	int		i = start, a = 0, r = 0, ni = 0;

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
	return (norm);
}

char	**normalize_command(char **tokens)
{
	int		total = count_tokens(tokens);
	char	**result = malloc(sizeof(char *) * (total + 10));
	int		i = 0, start = 0, ri = 0;

	while (i <= total)
	{
		if (!tokens[i] || is_pipe(tokens[i]))
		{
			char **segment = normalize_segment(tokens, start, i);
			int j = 0;
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
	int	i = 0;
	int	len = 0;

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
