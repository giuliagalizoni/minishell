/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:01:46 by ggalizon          #+#    #+#             */
/*   Updated: 2025/06/02 13:01:47 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	error_cleanup(t_msh *msh)
{
	perror("pipe failure in heredoc");
	cleanup(msh);
}

void	export_error(char *arg)
{
	write(2, "conchinha: export: `", 21);
	write(2, arg, ft_strlen(arg));
	write(2, "\': not a valid identifier\n", 26);
}

void	exit_process(t_msh *msh, t_command *command, char *arg, char *err_msg)
{
	int	status;

	status = msh->exit_status;
	if (status != 0)
		ft_perror(command->name, arg, status, err_msg);
	cleanup(msh);
	exit(status);
}

int	invalid_option_error(t_command *cmd, char *arg, int status)
{
	ft_putstr_fd("conchinha: ", STDERR_FILENO);
	ft_putstr_fd(cmd->name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": invalid option\n", STDERR_FILENO);
	return (status);
}

int	fork_error(t_command *cmd, t_msh *msh, int *pipe_fds)
{
	perror("minishell: fork failed");
	msh->exit_status = EXIT_FAILURE;
	if (cmd->index < msh->num_cmds - 1 && msh->num_cmds > 1)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
	return (-1);
}
