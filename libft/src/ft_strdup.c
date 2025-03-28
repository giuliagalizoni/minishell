/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:48:50 by marcampo          #+#    #+#             */
/*   Updated: 2024/06/11 12:50:37 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*t;
	size_t	i;

	i = 0;
	len = ft_strlen(s) + 1;
	t = (char *)malloc(len * sizeof(char));
	if (t == NULL)
		return (NULL);
	while (len > i)
	{
		t[i] = s[i];
		i++;
	}
	return (t);
}
