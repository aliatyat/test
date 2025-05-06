/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:49 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/05 20:09:32 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal_status;
// Signal handler function
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

int	init_shell(t_shell *shell, char **envp)
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->env = copy_env(envp);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	//signal(SIGPIPE, SIG_IGN);
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

	add_history(input);
	shell->input = input;
	shell->commands = parse_input(input, shell);
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
		printf("argc: %d\n", argc);
	(void)argv;
	if (!init_shell(&shell, envp))
	return (EXIT_FAILURE);
	//increment_shell_level(&shell);
	while (1)
	{	
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		shell.argv_z = argv[0];
		input = readline("minishell$ ");
		if (!input)
		{
			handle_eof(&shell);
			break ;
		}
		process_input(&shell, input);
		free(input);
		
	}
	cleanup_shell(&shell);
	return (g_signal_status);
}
