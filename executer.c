#include "includes/minishell.h"
// TODO
// split the function into process / child, see what needs to be passed around
// put the pids in the command struct
// flesh out error handling a bit more (free the command object at every
// error?)
// see if we can get rid of the prev_pipe_read_fd, i don't like it

void	wait_for_children(pid_t *pids, int num_cmds)
{
	int	i;
	int	status;

	i = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
}

void	child_process(t_command *cmd, int prev_pipe_read_fd, int *fd, int num_cmds)
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
	if (cmd->index < num_cmds - 1)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed for stdout redirection");
			exit(EXIT_FAILURE);
		}
		close(fd[1]);
	}
	// do we need a case for a single command?
	if (is_builtin(cmd->name))
		builtin_router(cmd);
	else
		execve(cmd->path, cmd->arguments, NULL);
	perror("execve failed");
	exit(EXIT_FAILURE);
}

void	parent_process(t_command *cmd, pid_t *pids, int pid, int *fd, int *prev_pipe_read_fd, int num_cmds)
{
	pids[cmd->index] = pid;
	if (*prev_pipe_read_fd != STDIN_FILENO)
		close(*prev_pipe_read_fd);
	if (cmd->index < num_cmds - 1)
	{
		close(fd[1]);
		*prev_pipe_read_fd = fd[0];
	}
}

void	process(t_command *cmd, int num_cmds)
{
	int	fd[2];
	//TODO move the pids to the cmd stuct
	pid_t	*pids;
	int	prev_pipe_read_fd;

	prev_pipe_read_fd = STDIN_FILENO;
	pids = malloc(num_cmds * sizeof(int));
	if (!pids)
		perror("malloc fail");


	if (pipe(fd) == -1)
		perror("pipe fail");

	while (cmd)
	{
		// if not last cmd, if
		if (cmd->index < num_cmds - 1 && num_cmds > 1)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe fail");
				free(pids);
				exit(EXIT_FAILURE);
			}
		}
		//TODO what to do when its more than one cmd?bash seems to just
		//eat it up
		if (num_cmds == 1 && is_equal(cmd->name, "exit"))
			exit_shell(cmd);
		pid_t pid = fork();
		if (pid == -1)
		{
			perror("fork fail");
			//some cleanup, close fds, free pids
		}
		else if (pid == 0)
			child_process(cmd, prev_pipe_read_fd, fd, num_cmds);
		else
			parent_process(cmd, pids, pid, fd, &prev_pipe_read_fd, num_cmds);
		cmd = cmd->pipe_next;
	}
	wait_for_children(pids, num_cmds);
	free(pids);
}
