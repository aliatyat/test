/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:28:10 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/01 14:38:47 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_input(char *input, t_shell *shell)
{
	char buffer[4096];
	size_t buf_index = 0;
	int in_single_quote = 0;
	int in_double_quote = 0;
	char *ptr = input;
	char *exit_status1;

	if (!input)
		return (NULL);

	while (*ptr && buf_index < sizeof(buffer) - 1)
	{
		if (*ptr == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			buffer[buf_index++] = *ptr++;
			continue ;
		}
		else if (*ptr == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			buffer[buf_index++] = *ptr++;
			continue ;
		}
		printf("fffff %ld\n", ptr - input);
		if (*ptr == '$' && !in_single_quote && ft_strncmp(ptr - 2, "<<",
				2) != 0)
		{
			ptr++; // Skip $
			if (*ptr == '?')
			{
				if (signal_status != 0)
				{
					exit_status1 = ft_itoa(signal_status);
				}
				else
				{
				exit_status1 = ft_itoa(shell->exit_status);
				}
				if (exit_status1)
				{
					ft_strlcpy(buffer + buf_index, exit_status1, sizeof(buffer)
					- buf_index);
					buf_index += ft_strlen(exit_status1);
					free(exit_status1);
				}
				ptr++;
				continue ;
			}
			else if (ft_isalpha(*ptr) || *ptr == '_')
			{
				char *var_start = ptr;
				while (ft_isalnum(*ptr) || *ptr == '_')
				ptr++;
				
				char *var_name = ft_substr(var_start, 0, ptr - var_start);
				char *value = get_env_value(shell->env, var_name);
				if (value)
				{
					ft_strlcpy(buffer + buf_index, value, sizeof(buffer)
					- buf_index);
					buf_index += ft_strlen(value);
				}
				free(var_name);
				continue ;
			}
			buffer[buf_index++] = '$';
		}
		else
		{
			buffer[buf_index++] = *ptr++;
		}
	}
	buffer[buf_index] = '\0';
	
	return (ft_strdup(buffer));
}
