/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:29:05 by marcampo          #+#    #+#             */
/*   Updated: 2024/06/11 12:29:06 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	d;
	const char		*t;

	d = (unsigned char)c;
	t = s + ft_strlen(s);
	if (*t == '\0' && d == '\0')
	{
		return ((char *)t);
	}
	while (t != s)
	{
		if (*t == d)
			return ((char *)t);
		t--;
	}
	if (*t == d)
		return ((char *)t);
	return (NULL);
}
