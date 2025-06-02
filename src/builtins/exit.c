/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:55:23 by ggalizon          #+#    #+#             */
/*   Updated: 2025/06/02 12:57:18 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exit_shell(t_msh *msh, t_command *command)
{
	int	arg_len;

	if (command != NULL)
	{
		arg_len = array_length((void **)command->arguments);
		if (arg_len > 1 && !is_num_string(command->arguments[1]))
			msh->exit_status = ft_perror(command->name, command->arguments[1],
					0, "numeric argument required");
		else if (arg_len > 2)
			msh->exit_status = ft_perror(command->name, NULL, 1,
					"too many arguments");
		if (arg_len == 2 && is_num_string(command->arguments[1]))
			msh->exit_status = ft_atoi(command->arguments[1]);
		else
			msh->exit_status = 0;
	}
	if (msh->num_cmds < 2)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	msh->exit = 1;
}
