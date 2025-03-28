/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:00:17 by marcampo          #+#    #+#             */
/*   Updated: 2024/06/11 14:25:30 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	num_words;

	num_words = 0;
	while (*s)
	{
		if (*s && *s != c)
		{
			num_words++;
			while (*s != c && *s)
				s++;
		}
		else
			s++;
	}
	return (num_words);
}

static void	*ft_free(char **lines, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		free(lines[i]);
		i++;
	}
	free(lines);
	return (NULL);
}

static char	**fill_array(size_t num_words, char **lines, char const *s, char c)
{
	size_t	i;
	char	*start;
	char	*end;
	size_t	line_size;

	i = 0;
	while (i < num_words)
	{
		if (*s != c)
		{
			start = (char *)s;
			while (*(s + 1) && *(s + 1) != c)
				s++;
			end = (char *)s;
			line_size = (end - start) + 2;
			lines[i] = malloc(line_size * sizeof(char));
			if (lines[i] == NULL)
				return (ft_free(lines, i));
			ft_strlcpy(lines[i], start, line_size);
			s++;
			i++;
		}
		s++;
	}
	return (lines);
}

char	**ft_split(char const *s, char c)
{
	size_t	num_words;
	char	**lines;

	num_words = count_words(s, c);
	lines = malloc((num_words + 1) * sizeof(char *));
	if (lines == NULL)
		return (NULL);
	lines = fill_array(num_words, lines, s, c);
	lines[num_words] = 0;
	return (lines);
}
