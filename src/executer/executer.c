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
		// TODO teardown
		perror("Error: failed to get status for the last command");
		msh->exit_status = -1;
	}
	(void)waited_pid;
}

int	single_parent_process(t_msh *msh)
{
	int	saved_stdout_fd;
	int	saved_stdin_fd;
	int	status;

	// TODO need some error checking somewhere?
	saved_stdout_fd = dup(STDOUT_FILENO);
	saved_stdin_fd = dup(STDIN_FILENO);
	if (msh->command->input_redirect)
		input_redirection(msh->command);
	if (msh->command->outfile)
		output_redirection(msh->command->outfile);
	status = builtin_router(msh);
	if (dup2(saved_stdin_fd, STDIN_FILENO) < 0 || dup2(saved_stdout_fd, STDOUT_FILENO) < 0)
		cleanup_on_error(msh, "dup2 failed");
	return (status);
}

void	child_process(t_msh *msh, int prev_pipe_read_fd, int *fd)
{
	if (prev_pipe_read_fd != STDIN_FILENO)
	{
		if (dup2(prev_pipe_read_fd, STDIN_FILENO) == -1)
		{
			//TODO teardown
			perror("dup2 fail for stdin redirection");
			exit(EXIT_FAILURE);
		}
		close(prev_pipe_read_fd);
	}
	if (msh->command->index < msh->num_cmds - 1)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			//TODO teardown
			perror("dup2 failed for stdout redirection");
			exit(EXIT_FAILURE);
		}
		close(fd[1]);
	}
	// ***	HEREDOC ***
	if (msh->command->is_heredoc)
	{
		dup2(msh->command->heredoc_fd, STDIN_FILENO);
		close(msh->command->heredoc_fd);
	}
	//TODO what to do with builtins?if i just move his code to process it
	//hangs. Mayb just copy it to the builtin router?
	if (msh->command->input_redirect)
		input_redirection(msh->command);
	if (msh->command->outfile)
		output_redirection(msh->command->outfile);
	if (is_builtin(msh->command->name))
		child_builtin(msh);
	else
	{
		execve(msh->command->path, msh->command->arguments, NULL);
		perror("command not found");
		exit(127); // TODO CLEANUP !!!!!
	}
}

void	parent_process(t_msh *msh, int *fd, int *prev_pipe_read_fd)
{
	if (*prev_pipe_read_fd != STDIN_FILENO)
		close(*prev_pipe_read_fd);
	if (msh->command->index < msh->num_cmds - 1)
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
	t_command	*first_command;

	if (!process_heredocs(msh))
		return (0);
	if (msh->num_cmds == 1 && is_builtin(msh->command->name))
		return (single_parent_process(msh));
	status = 0;
	first_command = msh->command;
	prev_pipe_read_fd = STDIN_FILENO;
	if (pipe(fd) == -1)
		perror("pipe fail");
	while (msh->command)
	{
		if (msh->command->index < msh->num_cmds - 1 && msh->num_cmds > 1)
		{
			if (pipe(fd) == -1)
			{
				//TODO cleanup cmd chain, print error, set exit
				// return 0 or 1 or whatever
				perror("pipe fail");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		msh->command->pid = pid;
		if (pid == -1)
		{
			//TODO cleanup cmd chain, print error, set exit
			// return 0 or 1 or whatev
			perror("fork fail");
		}
		else if (pid == 0)
			child_process(msh, prev_pipe_read_fd, fd);
		else
			parent_process(msh, fd, &prev_pipe_read_fd);
		msh->command = msh->command->pipe_next;
	}
	wait_for_children(msh, first_command);
	close(fd[0]);
	close(fd[1]);
	return (status);
}
