/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 20:11:02 by alalauty          #+#    #+#             */
/*   Updated: 2025/05/05 19:05:06 by alalauty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)

{
	const unsigned char	*s;
	unsigned char		*d;
	size_t				i;

	if (dst == NULL && src == NULL)
	{
		return (NULL);
	}
	i = 0;
	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	while (n > i)
	{
		d[i] = s[i];
		i++;
	}
	return (d);
}
/*
int	main(void)
{
	const unsigned char	src[];
	unsigned char		dst[4];

	src[] = "asd";
	
	ft_memcpy(dst, src, ft_strlen(src) + 1);
	printf("%s", dst);
	return (0);
}
*/
