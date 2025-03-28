/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:56:52 by marcampo          #+#    #+#             */
/*   Updated: 2024/06/11 12:59:22 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_ltrim(char const *s1, char const *set, size_t set_len)
{
	size_t		i;
	size_t		j;
	char		*s2;

	i = 0;
	j = 0;
	s2 = (char *)s1;
	while (*s2 && (j < set_len))
	{
		while (j < set_len)
		{
			if (s1[i] == set[j])
			{
				s2++;
				i++;
				j = 0;
			}
			else
				j++;
		}
	}
	return (s2);
}

static size_t	ft_rtrim(char *s2, char const *set, size_t set_len)
{
	size_t		i;
	size_t		j;
	size_t		len;

	j = 0;
	len = ft_strlen(s2);
	i = len - 1;
	while (len && (j < set_len))
	{
		while (j < set_len)
		{
			if (s2[i] == set[j])
			{
				len--;
				i--;
				j = 0;
			}
			else
				j++;
		}
	}
	return (len);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	size_t	set_len;
	size_t	i;
	char	*s2;
	char	*trimmed;

	set_len = ft_strlen(set);
	s2 = ft_ltrim(s1, set, set_len);
	len = ft_rtrim(s2, set, set_len);
	trimmed = (char *)malloc((len + 1) * sizeof(char));
	if (trimmed == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		trimmed[i] = s2[i];
		i++;
	}
	trimmed[len] = '\0';
	return (trimmed);
}
