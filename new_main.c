/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamaien <zjamaien@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:49 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/12 21:24:13 by zjamaien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal_status;

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
	add_history(input);
	shell->input = input;
	shell->commands = parse_input(input, shell);
	printf("shell  %d\n", shell->exit_status);
	if (shell->commands == NULL)
	{
		shell->exit_status = 2;
		return ;
	}
	if (shell->commands)
	{
		shell->exit_status = execute_command(shell->commands, shell);
		free_commands(shell->commands);
		shell->commands = NULL;
	}
}

void	cleanup_shell(t_shell *shell)
{
	free_env(shell->env);
}

static int	usage_minishell(char *argv, int argc)
{
	if (argv && argc > 1)
	{
		ft_putstr_fd("USAGE :./minishell no arg\n", 2);
		return (1);
	}
	return (0);
}


static int handle_ctrl_d(t_shell *shell ,char *input)
{
	if (!input)
	{
		handle_eof(shell);
		return (-1);
	}
	return (0);	
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;
	int	save_exit;

	save_exit = 0;
	if (usage_minishell(argv[1], argc) == 1)
		return (1);
	if (!init_shell(&shell, envp))
		return (EXIT_FAILURE);
	while (1)
	{
		handle_signal();
		shell.argv_z = argv[0];
		shell.exit_status = save_exit;
		input = readline("minishell$ ");
		if (handle_ctrl_d(&shell, input) == -1)
			break ;
		process_input(&shell, input);
		save_exit = shell.exit_status;
		close_all_pipes(shell.commands);
		free(input);
	}
	cleanup_shell(&shell);
	return (g_signal_status);
}
