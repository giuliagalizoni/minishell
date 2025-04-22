#include "includes/minishell.h"
// TODO
// split the function into process / child, see what needs to be passed around
// put the pids in the command struct
// flesh out error handling a bit more (free the command object at every
// error?)
// see if we can get rid of the prev_pipe_read_fd, i don't like it

int	wait_for_children(t_msh *msh, pid_t *pids)
{
	int	i;
	int	status;
	pid_t	waited_pid;

	i = 0;
	while (i < msh->num_cmds)
	{
		// TODO need to check if waitpid returns -1?
		// https://g.co/gemini/share/fdc126ab4f98
		waited_pid = waitpid(pids[i], &status, 0);
		if (waited_pid == -1)
			perror("I don't know what this error ir supposed to be"); // TODO
																	  // Review
																	  // this
		i++;
	}
	status = WEXITSTATUS(status);
	return (status);
}

static void	input_redirection(t_command *cmd)
{
	int	i;
	int	file;

	i = 0;
	while(cmd->input_redirect[i])
	{
		file = open(cmd->input_redirect[i], O_RDONLY);
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
	execve(msh->command->path, msh->command->arguments, NULL);
	perror("execve failed");
	exit(EXIT_FAILURE);
}

void	parent_process(t_msh *msh, pid_t *pids, int *fd, int *prev_pipe_read_fd)
{
	pids[msh->command->index] = msh->command->pid;
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
	pid_t	*pids;
	int	status;
	int	prev_pipe_read_fd;

	status = 0;
	prev_pipe_read_fd = STDIN_FILENO;
	pids = malloc(msh->num_cmds * sizeof(int));
	if (!pids)
		perror("malloc fail");
	if (pipe(fd) == -1)
		perror("pipe fail");
	while (msh->command)
	{
		// if not last cmd, if
		if (msh->command->index < msh->num_cmds - 1 && msh->num_cmds > 1)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe fail");
				free(pids);
				exit(EXIT_FAILURE);
			}
		}
		//TODO do we need a case for a single command?
		//TODO what to do when its more than one cmd?bash seems to just
		//eat it up
		if (is_builtin(msh->command->name))
		{
			// TODO what are we doing with the pipes and everything
			// here
			builtin_router(msh);
			msh->command = msh->command->pipe_next;
			// include parent process cleanup here?
		}
		else
		{
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
				parent_process(msh, pids, fd, &prev_pipe_read_fd);
			msh->command = msh->command->pipe_next;
		}
	}
	//what if the last command is a builtin
	status = wait_for_children(msh, pids);
	free(pids);
	close(fd[0]);
	close(fd[1]);
	return (status);
}
