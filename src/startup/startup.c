/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   startup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:01:08 by ggalizon          #+#    #+#             */
/*   Updated: 2025/06/02 13:01:09 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define RESET "\033[0m"

void	print_banner(void)
{
	int		fd;
	int		i;
	char	*line;

	i = 0;
	fd = open(BANNER_FILE_PATH, O_RDONLY);
	if (fd == -1)
		perror("banner file read error");
	printf("\n");
	line = get_next_line(fd);
	while (line)
	{
		printf("\033[38;5;%dm", 130 + i);
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
		i++;
	}
	printf("\nGiulia Galizoni Caversan && Mario Campos Castellano 2025\n");
	printf("%s", RESET);
	printf("\n");
	close(fd);
}
