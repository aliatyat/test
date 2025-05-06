/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:27:59 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/05 18:50:40 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "LIBFT/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

typedef struct s_command
{
	char				**args;
	int					in_fd;
	int					out_fd;
	struct s_command	*next;
}						t_command;

typedef struct s_shell
{
	char				**env;
	int					exit_status;
	int					last_fork;
	char 				*argv_z;
	t_command			*commands;
	char				*input;
}						t_shell;

extern int				g_signal_status;
// Environment functions
char					**copy_env(char **envp);
char					*get_env_value(char **env, char *name);
char					**update_env_var(char **env, char *name, char *value);
void					free_env(char **env);
char					**remove_env_var(char **env, char *var);
void					print_sorted_env(char **env, int fd);
char					*find_path(char **env, char *cmd);
char					**get_paths_from_env(char **env);
void					close_all_pipes(t_command *commands);
void					close_all_pipes1(t_command *commands);
char					**env_to_array(char **env);

// Command execution
int						execute_command(t_command *cmd, t_shell *shell);
int						execute_builtin(t_command *cmd, t_shell *shell);
int						execute_external(t_command *cmd, t_shell *shell);

// Builtins
int						ft_cd(t_command *cmd, t_shell *shell);
char					*ft_echo(t_command *cmd, t_shell *shell);
int						ft_env(t_shell *shell);
int					ft_exit(t_command *cmd, t_shell *shell);
int						ft_export(t_command *cmd, t_shell *shell);
int						ft_pwd(t_command *cmd);
int						ft_unset(t_command *cmd, t_shell *shell);

// Parsing
t_command				*parse_input(char *input, t_shell *shell);
void					free_commands(t_command *cmd);

// Utils
void					free_2d_array(char **array);
char					*ft_strjoin3(const char *s1, const char *s2,
							const char *s3);
char					*ft_strjoin4(const char *s1, const char *s2,
							const char *s3);
char					*ft_strndup(char *s, size_t n);
char					**ft_realloc_strarr(char **arr, size_t new_size);
int						ft_error(char *cmd, char *msg, int status);
int						ft_perror(char *cmd, int status);
void					free_split(char **array);
char					**ft_split_shell(char *str, char delim);
int						is_builtin(char *cmd);

// Redirection
int						handle_redirection1(t_command *cmd, t_shell *shell);
int						has_redirection(char *cmd);
int						handle_heredoc(t_command *cmd, char *delimiter,
							t_shell *shell);

void					handle_sigint(int sig);

char					**ft_split_pipes(char *input);
int						execute_pipeline(t_command *cmd, t_shell *shell);
char					**remove_null_args(char **args);
char					**split_with_redirections(char *input);
int						process_echo_arg(char *arg, t_shell *shell, int out_fd);
void					print_env_var(char *var_start, char **arg,
							t_shell *shell, int out_fd);
char					**split_ex(char *input);
char					*ft_ex(char *cmd, t_shell *shell);
char					*expand_input(char *input, t_shell *shell);
int						has_unclosed_quotes(char *str);
int						handle_child_process(t_command *cmd, t_shell *shell,
							char *full_path);

char					**split_tokens(char *input);
char					**normalize_command(char **tokens);
char					*join_tokens_back(char **tokens);
int						is_invalid_syntax(char *input, t_shell *shell);
void	wait_for_children(t_shell *shell);
char	*get_next_token_here(char **str, char delim);


#endif