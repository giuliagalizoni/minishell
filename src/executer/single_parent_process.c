/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_parent_process.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:59:17 by ggalizon          #+#    #+#             */
/*   Updated: 2025/06/02 12:59:18 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	save_standard_fds(int *saved_stdin_fd, int *saved_stdout_fd)
{
	*saved_stdout_fd = dup(STDOUT_FILENO);
	if (*saved_stdout_fd < 0)
	{
		perror("minishell: dup failed for saved_stdout_fd");
		return (0);
	}
	*saved_stdin_fd = dup(STDIN_FILENO);
	if (*saved_stdin_fd < 0)
	{
		perror("minishell: dup failed for saved_stdin_fd");
		close(*saved_stdout_fd);
		return (0);
	}
	return (1);
}

static int	apply_redirections_and_run_builtin(t_msh *msh)
{
	int	redir_success;

	redir_success = 1;
	if (msh->command->heredoc_is_final)
	{
		if (msh->command->is_heredoc && msh->command->heredoc_fd[0] != -1)
		{
			if (dup2(msh->command->heredoc_fd[0], STDIN_FILENO) == -1)
				perror("minishell: dup2 heredoc failed");
			else
				redir_success = (close(msh->command->heredoc_fd[0]) != -1);
			if (redir_success == 0)
				perror("minishell: close heredoc_fd failed");
		}
	}
	else if (msh->command->input_redirect)
		if (!input_redirection(msh->command))
			redir_success = 0;
	if (msh->command->outfile && redir_success)
		if (!output_redirection(msh->command->outfile))
			redir_success = 0;
	if (!redir_success)
		return (EXIT_FAILURE);
	return (builtin_router(msh, msh->command));
}

static int	restore_standard_fds(int s_in, int s_out, int current_status)
{
	int	final_status;

	final_status = current_status;
	if (dup2(s_in, STDIN_FILENO) < 0)
	{
		perror("minishell: dup2 failed to restore stdin");
		dup2(s_out, STDOUT_FILENO);
		final_status = EXIT_FAILURE;
	}
	close(s_in);
	if (dup2(s_out, STDOUT_FILENO) < 0)
	{
		perror("minishell: dup2 failed to restore stdout");
		final_status = EXIT_FAILURE;
	}
	close(s_out);
	return (final_status);
}

int	single_parent_process(t_msh *msh)
{
	int	saved_stdout_fd;
	int	saved_stdin_fd;
	int	status;

	if (!save_standard_fds(&saved_stdin_fd, &saved_stdout_fd))
		return (EXIT_FAILURE);
	status = apply_redirections_and_run_builtin(msh);
	status = restore_standard_fds(saved_stdin_fd, saved_stdout_fd, status);
	return (status);
}
