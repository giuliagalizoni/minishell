#include "includes/minishell.h"
// TODO
// split the function into process / child, see what needs to be passed around
// put the pids in the command struct
// flesh out error handling a bit more (free the command object at every
// error?)
// see if we can get rid of the prev_pipe_read_fd, i don't like it

int	wait_for_children(t_command *first_command)
{
	t_command *command;
	int	status;
	int	final_status;
	pid_t	waited_pid;

	// TODO review this a bit according to this
	// https://gemini.google.com/app/a5ea8cc40aba5f5e
	// but maybe it's fine like this
	command = first_command;
	while (command)
	{
		waited_pid = waitpid(command->pid, &status, 0);
		if (waited_pid == -1)
			perror("waitpid failed"); // TODO cleanup?
		command = command->pipe_next;
	}
	if (WIFEXITED(status))
		final_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		final_status = 128 + WTERMSIG(status);
	else
		final_status = -1;
	return (final_status);
}

static void	input_redirection(t_command *command)
{
	int	i;
	int	file;

	i = 0;
	while(command->input_redirect[i])
	{
		file = open(command->input_redirect[i], O_RDONLY);
		if (file == -1)
			perror("Bad file descriptor");// cleanup routine here
		dup2(file, 0);
		close(file);
		i++;
	}
}

static void	output_redirection(t_outfile *outfile)
{
	int	file;

	while(outfile)
	{
		if (outfile->is_append)
			file = open(outfile->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			file = open(outfile->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (file == -1)
			perror("Bad file descriptor");// cleanup routine here
		dup2(file, 1);
		close(file);
		outfile = outfile->next;
	}
}

void	child_process(t_msh *msh, int prev_pipe_read_fd, int *fd)
{

	// TODO do i really need this prev_pipe_read_fd
	// if it's not the first cmd, redirect input
	if (prev_pipe_read_fd != STDIN_FILENO)
	{
		if (dup2(prev_pipe_read_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 fail for stdin redirection");
			exit(EXIT_FAILURE);
		}
		close(prev_pipe_read_fd);
	}
	// if it's not the last cmd, redirect output
	if (msh->command->index < msh->num_cmds - 1)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed for stdout redirection");
			exit(EXIT_FAILURE);
		}
		close(fd[1]);
	}
	//TODO what to do with builtins?if i just move his code to process it
	//hangs. Mayb just copy it to the builtin router?
	if (msh->command->input_redirect)
		input_redirection(msh->command);
	if (msh->command->outfile)
		output_redirection(msh->command->outfile);
	if (is_builtin(msh->command->name))
		builtin
	execve(msh->command->path, msh->command->arguments, NULL);
	perror("execve failed");
	exit(EXIT_FAILURE);
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
	int	fd[2];
	//TODO move the pids to the cmd stuct
	int	status;
	int	prev_pipe_read_fd;
	t_command	*first_command;

	status = 0;
	prev_pipe_read_fd = STDIN_FILENO;
	if (pipe(fd) == -1)
		perror("pipe fail");
	first_command = msh->command;
	// if only one cmd, check if its a builtin and execute it
	// TODO narrow this to parent-only processes?
	if (msh->num_cmds == 1 && is_builtin(msh->command->name))
			builtin_router(msh);
	while (msh->command)
	{
		// if not last cmd, if
		if (msh->command->index < msh->num_cmds - 1 && msh->num_cmds > 1)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe fail");
				exit(EXIT_FAILURE);
			}
		}
		//TODO do we need a case for a single command?
		//TODO what to do when its more than one cmd?bash seems to just
		//eat it up
		/*
		if (is_builtin(msh->command->name))
		{
			// TODO what are we doing with the pipes and everything
			// here
			builtin_router(msh);
			msh->command = msh->command->pipe_next;
			// include parent process cleanup here?
		}
		else
		*/
		pid_t pid = fork();
		msh->command->pid = pid;
		if (pid == -1)
		{
			perror("fork fail");
			//some cleanup, close fds, free pids
		}
		else if (pid == 0)
			child_process(msh, prev_pipe_read_fd, fd);
		else
			parent_process(msh, fd, &prev_pipe_read_fd);
		msh->command = msh->command->pipe_next;
	}
	//what if the last command is a builtin
	status = wait_for_children(first_command);
	printf("status: %d\n", status);
	close(fd[0]);
	close(fd[1]);
	return (status);
}
