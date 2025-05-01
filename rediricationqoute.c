char	**split_with_redirections(char *input)
{
	char	**tokens;
	char	*ptr;
	char	*start;
	int		i;
	int		in_quotes;
	char	quote_char;

	tokens = malloc(sizeof(char *) * 100);
	if (!tokens)
		return (NULL);
	i = 0;
	ptr = input;
	in_quotes = 0;
	quote_char = 0;
	while (*ptr)
	{
		while (*ptr == ' ' || *ptr == '\t')
			ptr++;
		if (!*ptr)
			break ;
		start = ptr;
		while (*ptr)
		{
			if (!in_quotes && (*ptr == '\'' || *ptr == '"'))
			{
				in_quotes = 1;
				quote_char = *ptr;
			}
			else if (in_quotes && *ptr == quote_char)
				in_quotes = 0;
			else if (!in_quotes && (*ptr == ' ' || *ptr == '\t'))
				break ;
			else if (!in_quotes && (*ptr == '>' || *ptr == '<') && ptr == start)
			{
				if (*(ptr + 1) == *ptr) // >> or <<
					ptr += 2;
				else
					ptr += 1;
				break ;
			}
			ptr++;
		}
		tokens[i++] = ft_strndup(start, ptr - start);
	}
	tokens[i] = NULL;
	return (tokens);
}
