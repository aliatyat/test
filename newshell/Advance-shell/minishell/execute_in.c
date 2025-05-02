/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_in.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:19 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/01 18:26:57 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	int	i;

	char *builtins[] = {
		"cd", "echo", "pwd", "export", "unset", "env", "exit", NULL};
	i = 0;
	if (!cmd)
		return (0);
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	handle_common_builtins(char *command, t_command *cmd, t_shell *shell)
{
	if (ft_strcmp(command, "echo") == 0)
	{
		ft_echo(cmd, shell);
		return (shell->exit_status = 0);
	}
	else if (ft_strcmp(command, "pwd") == 0)
	{
		shell->exit_status = ft_pwd(cmd);
		return (shell->exit_status);
	}
	else if (ft_strcmp(command, "env") == 0)
	{
		shell->exit_status = ft_env(shell);
		return (shell->exit_status);
	}
	return (0);
}

int	handle_special_builtins(char *command, t_command *cmd, t_shell *shell)
{
	if (ft_strcmp(command, "cd") == 0)
	{
		shell->exit_status = ft_cd(cmd, shell);
		return (shell->exit_status);
	}
	else if (ft_strcmp(command, "export") == 0)
	{
		shell->exit_status = ft_export(cmd, shell);
		return (shell->exit_status);
	}
	else if (ft_strcmp(command, "unset") == 0)
	{
		shell->exit_status = ft_unset(cmd, shell);
		return (0);
	}
	else if (ft_strcmp(command, "exit") == 0)
	{
		shell->exit_status = ft_exit(cmd, shell);
		return (shell->exit_status);
	}
	return (0);
}

int	execute_builtin(t_command *cmd, t_shell *shell)
{
	char	*command;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	command = cmd->args[0];
	if (handle_common_builtins(command, cmd, shell))
		return (shell->exit_status);
	return (handle_special_builtins(command, cmd, shell));
}
static int	is_redirection(char *s)
{
	return (!ft_strcmp(s, ">") || !ft_strcmp(s, ">>") || !ft_strcmp(s, "<") || !ft_strcmp(s,
			"<<"));
}
int	ft_strslen(char **strs)
{
	int	i = 0;

	while (strs && strs[i])
		i++;
	return (i);
}
static char	**normalize_segment1(char **tokens, int start, int end)
{
	char	**norm;
	char	**args;
	char	**redir;
	char	*cmd;
	int		i;
	int		a;
	int		r;
	int		ni;
	int		raw_i;

	norm = malloc(sizeof(char *) * (end - start + 10));
	args = malloc(sizeof(char *) * (end - start + 10));
	redir = malloc(sizeof(char *) * (end - start + 10));
	if (!norm || !args || !redir)
		return (NULL);
	cmd = NULL;
	a = 0;
	r = 0;
	ni = 0;
	i = start;
	while (i < end)
	{
		if (is_redirection(tokens[i]) && (i + 1 < end))
		{
			redir[r++] = tokens[i++];
			redir[r++] = tokens[i];
		}
		else if (!cmd)
			cmd = tokens[i];
		else
			args[a++] = tokens[i];
		i++;
	}
	if (!cmd)
	{
		char **raw = malloc(sizeof(char *) * (end - start + 1));
		if (!raw)
			return (NULL);
		raw_i = 0;
		i = start;
		while (i < end)
			raw[raw_i++] = tokens[i++];
		raw[raw_i] = NULL;
		free(args);
		free(redir);
		free(norm);
		return (raw);
	}
	norm[ni++] = cmd;
	i = 0;
	while (i < a)
		norm[ni++] = args[i++];
	i = 0;
	while (i < r)
		norm[ni++] = redir[i++];
	norm[ni] = NULL;
	free(args);
	free(redir);
	return (norm);
}

int	execute_command(t_command *cmd, t_shell *shell)
{


	
	int	i;
	int	x;
	int	z;

	i = 0;
	x = 0;
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	
		
	while (shell->input[i])
	{
		if (shell->input[i] == '|' || shell->input[i] == '>'
			|| shell->input[i] == '<')
		{
			x = 1;
			break ;
		}
		i++;
	}
	if (cmd->next == NULL)
	
		if (!cmd->next && ft_strcmp(cmd->args[0], "cd") == 0)
			return (execute_builtin(cmd, shell));
	
	else if (!cmd->next && (is_builtin(cmd->args[0]) && x == 0))
	{
		execute_builtin(cmd, shell);
		return (shell->exit_status);
	}
	return (execute_pipeline(cmd, shell));
}
