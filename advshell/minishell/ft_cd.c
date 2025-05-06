/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:28 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/05 17:27:36 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_cd_target(t_command *cmd, t_shell *shell, char **target_dir)
{
	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0)
	{
		*target_dir = get_env_value(shell->env, "HOME");
		if (!*target_dir)
			return (ft_error("cd", "HOME not set", 1));
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		*target_dir = get_env_value(shell->env, "OLDPWD");
		if (!*target_dir)
			return (ft_error("cd", "OLDPWD not set", 1));
		ft_putendl_fd(*target_dir, STDOUT_FILENO);
	}
	else
	{
		*target_dir = cmd->args[1];
	}
	return (0);
}

int	ft_cd(t_command *cmd, t_shell *shell)
{
	char	*target_dir;
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];
	int		ret;

	ret = handle_cd_target(cmd, shell, &target_dir);
	if (ret != 0)
		return (ret);
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		return (ft_perror("cd 1", 1));
	if (chdir(target_dir) != 0)
		return (ft_perror("cd 2", 1));
	if (!getcwd(new_pwd, sizeof(new_pwd)))
		return (ft_perror("cd 3", 1));
	shell->env = update_env_var(shell->env, "OLDPWD", old_pwd);
	shell->env = update_env_var(shell->env, "PWD", new_pwd);
	return (0);
}
