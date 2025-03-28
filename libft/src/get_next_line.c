/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:37:08 by marcampo          #+#    #+#             */
/*   Updated: 2024/07/05 14:38:45 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*read_line(int fd, char *buf, char *remainder);
char	*extract_line(char *line);

char	*get_next_line(int fd)
{
	static char	*remainder[1024];
	char		*line;
	char		*buf;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	line = read_line(fd, buf, remainder[fd]);
	free(buf);
	buf = NULL;
	if (!line)
	{
		free(remainder[fd]);
		remainder[fd] = NULL;
		return (NULL);
	}
	remainder[fd] = extract_line(line);
	return (line);
}

char	*read_line(int fd, char *buf, char *remainder)
{
	ssize_t		bytes_read;
	char		*tmp;

	bytes_read = 1;
	if (remainder)
		if (ft_strchr(remainder, '\n'))
			return (remainder);
	while (1)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (!bytes_read)
			break ;
		if (bytes_read == -1)
			return (NULL);
		buf[bytes_read] = '\0';
		if (!remainder)
			remainder = ft_strdup("");
		tmp = remainder;
		remainder = ft_strjoin(tmp, buf);
		free(tmp);
		tmp = NULL;
		if (ft_strchr(buf, '\n'))
			break ;
	}
	return (remainder);
}

char	*extract_line(char *line)
{
	char		*remainder;
	size_t		i;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == '\0' || line[1] == '\0')
		return (NULL);
	remainder = ft_substr(line, i + 1, ft_strlen(line) - i);
	if (*remainder == '\0')
	{
		free(remainder);
		remainder = NULL;
	}
	line[i + 1] = '\0';
	return (remainder);
}
