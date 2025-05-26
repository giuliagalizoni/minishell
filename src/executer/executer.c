#include "../includes/minishell.h"

void	wait_for_children(t_msh *msh, t_command *first_command)
{
	t_command	*command;
	int			status;
	pid_t		waited_pid;

	status = 0;
	command = first_command;
	while (command)
	{
		waited_pid = waitpid(command->pid, &status, 0);
		command = command->pipe_next;
	}
	if (WIFEXITED(status))
		msh->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		msh->exit_status = 128 + WTERMSIG(status);
	else
	{
		msh->exit_status = -1;
		ft_perror(command, NULL, msh->exit_status, "failed to get status for the last command");
		clear_command_chain(first_command);
	}
	(void)waited_pid;
}

int	single_parent_process(t_msh *msh)
{
	int	saved_stdout_fd;
	int	saved_stdin_fd;
	int	status;

	saved_stdout_fd = dup(STDOUT_FILENO);
	if (saved_stdout_fd < 0)
	{
		perror("minishell: dup failed for saved_stdout_fd");
		return (EXIT_FAILURE); // Or an appropriate error status
	}
	saved_stdin_fd = dup(STDIN_FILENO);
	if (saved_stdin_fd < 0)
	{
		perror("minishell: dup failed for saved_stdin_fd");
		close(saved_stdout_fd); // Clean up the successfully duped fd
		return (EXIT_FAILURE); // Or an appropriate error status
	}

	// TODO need some error checking somewhere? -> dup calls checked above.
	if (msh->command->input_redirect)
		input_redirection(msh->command);
	if (msh->command->outfile)
		output_redirection(msh->command->outfile);
	status = builtin_router(msh, msh->command);


	// Restore stdin and stdout
	if (dup2(saved_stdin_fd, STDIN_FILENO) < 0)
	{
		perror("minishell: dup2 failed to restore stdin");
		// Attempt to restore stdout anyway, then record error
		dup2(saved_stdout_fd, STDOUT_FILENO); // Best effort
		status = EXIT_FAILURE; // Ensure exit status reflects the error
	}
	close(saved_stdin_fd);
	if (dup2(saved_stdout_fd, STDOUT_FILENO) < 0)
	{
		perror("minishell: dup2 failed to restore stdout");
		status = EXIT_FAILURE; // Ensure exit status reflects the error
	}
	close(saved_stdout_fd);
	return (status);
}

void	child_process(t_msh *msh, t_command *command, int prev_pipe_read_fd, int *fd)
{
	char **envp;

	if (prev_pipe_read_fd != STDIN_FILENO)
	{
		if (dup2(prev_pipe_read_fd, STDIN_FILENO) == -1)
			exit_process(msh, command, NULL, "dup 2 fail for stdin redirection", EXIT_FAILURE);
		close(prev_pipe_read_fd);
	}
	if (command->index < msh->num_cmds - 1)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			exit_process(msh, command, NULL,  "dup 2 fail for stdout redirection", EXIT_FAILURE);
		close(fd[1]);
	}
	// ***	HEREDOC ***
	if (command->heredoc_is_final)
	{
		if (command->is_heredoc && command->heredoc_fd != -1)
		{
			if (dup2(command->heredoc_fd, STDIN_FILENO) == -1)
				exit_process(msh, command, NULL, "dup 2 fail for heredoc redirection", EXIT_FAILURE);
			close(command->heredoc_fd);
		}
	}
	else if (command->input_redirect && command->input_redirect[0] != NULL)
		if (!input_redirection(command)) // TODO we r printing twice
			exit_process(msh, command, command->input_redirect[0], NULL, EXIT_FAILURE);
	if (command->outfile)
		if (!output_redirection(command->outfile))
			exit_process(msh, command, NULL, "output redirection failed", EXIT_FAILURE);
	if (is_builtin(command->name))
		child_builtin(msh, command);
	if (command->path && is_directory(command->path))
		exit_process(msh, command, NULL, "Is a directory", 126);
	if (command->path && access(command->path, X_OK))
		exit_process(msh, command, NULL, "Permission denied", 126);
	else
	{
		if (!command->name)
			exit_process(msh, command, NULL,  NULL, EXIT_SUCCESS);
		if (!command->path)
			exit_process(msh, command, NULL, "command not found", 127);
		envp = myenv_to_envp(msh->myenv);
		if (execve(command->path, command->arguments, envp) == -1)
		{
			//TODO stuck here
			free_arr((void **)envp);
			exit_process(msh, command, NULL, "command not found", 127);
		}
		exit(EXIT_SUCCESS);
	}
}

void	parent_process(t_msh *msh, t_command *command, int *fd, int *prev_pipe_read_fd)
{
	if (*prev_pipe_read_fd != STDIN_FILENO)
		close(*prev_pipe_read_fd);
	if (command->index < msh->num_cmds - 1)
	{
		close(fd[1]);
		*prev_pipe_read_fd = fd[0];
	}
}

int	process(t_msh *msh)
{
	int			fd[2];
	int			status;
	int			prev_pipe_read_fd;
	pid_t		pid;
	t_command	*command;

	// If there's no command (e.g., empty line or parsing error), return 0.
	if (!msh->command)
	 // TODO maybe set exit status?
		return (0);

	if (!process_heredocs(msh))
		return (0);
	if (msh->num_cmds == 1 && is_builtin(msh->command->name))
		return (single_parent_process(msh));
	status = 0;
	command = msh->command;
	prev_pipe_read_fd = STDIN_FILENO;
	// Initialize fd outside the loop for the first pipe, and for closing if loop doesn't run.
	// However, pipe(fd) is best inside the loop if it's per-pipe segment.
	// For now, the existing single fd array is used and repiped.

	while (command)
	{
		if (command->index < msh->num_cmds - 1 && msh->num_cmds > 1)
		{
			if (pipe(fd) == -1)
			{
				perror("minishell: pipe failed");
				msh->exit_status = EXIT_FAILURE; // Set exit status
				if (prev_pipe_read_fd != STDIN_FILENO)
					close(prev_pipe_read_fd); // Close previous read end if not stdin
				break; // Exit the loop, proceed to wait_for_children and return
			}
		}
		pid = fork();
		command->pid = pid;
		if (pid == -1)
		{
			perror("minishell: fork failed");
			msh->exit_status = EXIT_FAILURE; // Set exit status
			if (command->index < msh->num_cmds - 1 && msh->num_cmds > 1) // If pipe was created for this command
			{
				close(fd[0]);
				close(fd[1]);
			}
			if (prev_pipe_read_fd != STDIN_FILENO)
				close(prev_pipe_read_fd); // Close previous read end if not stdin
			break; // Exit the loop
		}
		else if (pid == 0)
			child_process(msh, command, prev_pipe_read_fd, fd);
		else
			parent_process(msh, command, fd, &prev_pipe_read_fd);
		command = command->pipe_next;
	}
	// After the loop, prev_pipe_read_fd may hold the read end of the last pipe.
	// It should be closed by the main shell process if it wasn't stdin.
	if (prev_pipe_read_fd != STDIN_FILENO)
		close(prev_pipe_read_fd);

	wait_for_children(msh, msh->command);
	// The individual pipe FDs (fd[0], fd[1]) should have been managed (closed or passed on)
	// by child_process and parent_process within the loop.
	// Removing the general close(fd[0]); close(fd[1]); here.
	return (status); // msh->exit_status holds the more important status
}
