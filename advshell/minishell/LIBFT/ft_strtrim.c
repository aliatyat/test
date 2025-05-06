/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 17:18:47 by alalauty          #+#    #+#             */
/*   Updated: 2025/04/30 22:13:20 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char c, const char *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*trimmed_str;

	start = 0;
	if (!s1 || !set)
		return (NULL);
	while (s1[start] && is_in_set(s1[start], set))
		start++;
	end = ft_strlen(s1);
	while (end > start && is_in_set(s1[end - 1], set))
		end--;
	trimmed_str = (char *)malloc(end - start + 1);
	if (!trimmed_str)
	{
		free(trimmed_str);
		return (NULL);
	}
	ft_strncpy(trimmed_str, (char *)s1 + start, end - start);
	trimmed_str[end - start] = '\0';
	return (trimmed_str);
}

/*
int	main(void) {
    char *result;

    result = ft_strtrim("---Hello, World!---", "-");

    printf("Trimmed String: '%s'\n", result);

   
    free(result);

    return (0);
}
*/
