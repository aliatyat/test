#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int	is_special(const char *s)
{
	return (!strncmp(s, ">>", 2) || !strncmp(s, "<<", 2)
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
		// Skip spaces
		while (isspace(input[i]))
			i++;
		if (!input[i])
			break;

		// Handle quoted strings
		if (input[i] == '\'' || input[i] == '"')
		{
			quote_char = input[i];
			j = 0;
            i++;
			while (input[i + j] && input[i + j] != quote_char)
				j++;
            if (input[i + j] == quote_char)
                j++;
			tokens[k] = strndup(&input[i], j);
            tokens[k] = ft_strjoin("\"",tokens[k]);
			i += j; // Skip closing quote
            printf("tokens = %s  %d\n", tokens[k], j);
            k++;
           
		}
		// Handle << or >>
		else if (!strncmp(&input[i], ">>", 2) || !strncmp(&input[i], "<<", 2))
		{
			tokens[k++] = strndup(&input[i], 2);
			i += 2;
		}
		// Handle single-character operators
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
		{
			tokens[k++] = strndup(&input[i], 1);
			i++;
		}
		// Handle normal words
		else
		{
			j = 0;
			while (input[i + j] && !isspace(input[i + j])
				&& strncmp(&input[i + j], ">>", 2)
				&& strncmp(&input[i + j], "<<", 2)
				&& input[i + j] != '>' && input[i + j] != '<' && input[i + j] != '|')
				j++;
			tokens[k++] = strndup(&input[i], j);
			i += j;
		}
	}
	tokens[k] = NULL;
	return tokens;
}


static int	is_redirection(const char *s)
{
	return (!strcmp(s, ">") || !strcmp(s, ">>")
		|| !strcmp(s, "<") || !strcmp(s, "<<"));
}

static int	is_pipe(const char *s)
{
	return (!strcmp(s, "|"));
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
	// No command: return original range (no normalization)
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
			free(segment); // we only free array, not contents (shared)
			if (tokens[i])
				result[ri++] = tokens[i]; // copy the pipe token "|"
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
		len += strlen(tokens[i++]) + 1; // +1 for space
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



int main(int argc, char **argv)
{
    char **pipe_commands;
    while (1)
    {
    char *inputline = readline("enter command> ");

    char **input1 = split_tokens(inputline);
    int x = 0;
    x = 0;
    while (input1[x])
    {
        printf("split tokens %d: %s\n", x, input1[x]);
        x++;
    }
   
   
    char **pipe_commands = normalize_command(input1);
    char *joined = join_tokens_back(pipe_commands);
    printf("Joined: %s\n", joined);
    free(joined);
    }
    return (0);
}   
