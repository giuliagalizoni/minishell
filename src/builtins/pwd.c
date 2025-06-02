/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:58:53 by ggalizon          #+#    #+#             */
/*   Updated: 2025/06/02 12:58:53 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	pwd(t_command *command)
{
	int		status;
	char	pwd[PATH_MAX];

	status = 0;
	if (command->arguments[1] && command->arguments[1][0] == '-')
		return (invalid_option_error(command, command->arguments[1], 1));
	if (getcwd(pwd, PATH_MAX) != NULL)
		printf("%s\n", pwd);
	else
		status = 1;
	return (status);
}
