char	*get_next_token(char **str, char delim)
{
	const char	*start;
	char		*token;
	int			in_quotes;
	char		quote_char;
	int			len;

	in_quotes = 0;
	quote_char = 0;
	len = 0;

	// Skip leading delimiters and whitespace
	while (**str && (**str == delim || **str == ' ' || **str == '\t'))
		(*str)++;
	start = *str;

	// Count token length while respecting quotes
	while (**str)
	{
		if (!in_quotes && (**str == '\'' || **str == '"'))
		{
			quote_char = **str;
			in_quotes = 1;
		}
		else if (in_quotes && **str == quote_char)
			in_quotes = 0;
		else if (!in_quotes && **str == delim)
			break;
		(*str)++;
		len++;
	}

	// Allocate and copy token
	token = malloc((*str - start) + 1);
	if (!token)
		return (NULL);
	ft_strlcpy(token, start, (*str - start) + 1);
	return (token);
}
