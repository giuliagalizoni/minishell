/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:55:26 by marcampo          #+#    #+#             */
/*   Updated: 2024/06/11 12:55:27 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*ptr;
	size_t	i;

	i = 0;
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	ptr = (char *)malloc((len_s1 + len_s2 + 1) * sizeof(unsigned char));
	if (ptr == NULL)
		return (NULL);
	while (i < len_s1)
	{
		ptr[i] = s1[i];
		i++;
	}
	while (i < (len_s1 + len_s2))
	{
		ptr[i] = s2[i - len_s1];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
