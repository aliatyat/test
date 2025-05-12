
#define AC n[0]
#define R n[1]
#define START n[2]


void	fill_args_and_redir(char **tokens, char **args, char **redir,
		char **cmd)
{
	int	n[3];

	AC = 0;
	R = 0;
	START = -1;
	while (++START < count_tokens2(tokens))
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
	}
	null_terminate(redir, args, AC++, R++);
}
