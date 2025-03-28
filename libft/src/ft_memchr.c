/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:36:46 by marcampo          #+#    #+#             */
/*   Updated: 2024/06/11 12:37:56 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	d;
	unsigned char	*r;
	size_t			i;

	i = 0;
	d = (unsigned char)c;
	r = (unsigned char *)s;
	while (i < n)
	{
		if (r[i] == d)
			return ((void *)(s + i));
		i++;
	}
	return (NULL);
}
