/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:49 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/09 17:39:12 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal_status;

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	if (rl_on_new_line() == -1)
		write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal_status = 128 + SIGINT;
}

void	handle_signal(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

int	init_shell(t_shell *shell, char **envp)
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->env = copy_env(envp);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	if (!shell->env)
	{
		ft_putendl_fd("minishell: environment copy failed", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	set_initial_pwd(t_shell *shell)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		shell->env = update_env_var(shell->env, "PWD", cwd);
	return (1);
}

void	process_input(t_shell *shell, char *input)
{
	int	g;
	int	i;

	add_history(input);
	shell->input = input;
	printf("input: %s\n", shell->input);
	shell->commands = parse_input(input, shell);
	i = 0;
	if (shell->commands == NULL)
		return ;
	if (shell->commands)
	{
		shell->exit_status = execute_command(shell->commands, shell);
		free_commands(shell->commands);
		shell->commands = NULL;
	}
}

void	handle_eof(t_shell *shell)
{
	g_signal_status = 0;
	ft_putendl_fd("exit", STDOUT_FILENO);
}

void	cleanup_shell(t_shell *shell)
{
	free_env(shell->env);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;

	(void)argc;
	(void)argv;
	if (!init_shell(&shell, envp))
		return (EXIT_FAILURE);
	while (1)
	{
		handle_signal();
		shell.argv_z = argv[0];
		input = readline("minishell$ ");
		if (!input)
		{
			handle_eof(&shell);
			break ;
		}
		process_input(&shell, input);
		close_all_pipes(shell.commands);
		free(input);
	}
	cleanup_shell(&shell);
	return (g_signal_status);
}
