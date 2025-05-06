/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:31:36 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/05 19:03:35 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char *restrict str, const char *restrict delim)
{
	static char	*next;
	char		*token;

	if (str)
		next = str;
	if (!next)
		return (NULL);
	while (*next && ft_strchr(delim, *next))
		next++;
	if (!*next)
		return (NULL);
	token = next;
	while (*next && !ft_strchr(delim, *next))
		next++;
	if (*next)
	{
		*next = '\0';
		next++;
	}
	return (token);
}
