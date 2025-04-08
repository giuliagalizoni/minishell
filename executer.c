#include "includes/minishell.h"
// TODO
// split the function into process / child, see what needs to be passed around
// put the pids in the command struct
// flesh out error handling a bit more (free the command object at every
// error?)
// see if we can get rid of the prev_pipe_read_fd, i don't like it

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
	execve(cmd->name, cmd->arguments, NULL);
	perror("execve failed");
	exit(EXIT_FAILURE);
}

void	wait_for_children(pid_t *pids, int num_cmds)
{
	int	i;

	i = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
}

void	process(t_command *cmd)
{
	int	fd[2];
	//TODO move the pids to the cmd stuct
	pid_t	*pids;
	int	status;
	int	prev_pipe_read_fd;
	t_cmd_meta	cmd_meta;

	cmd_meta = cmd->cmd_meta;
	prev_pipe_read_fd = STDIN_FILENO;
	pids = malloc(cmd->cmd_meta.num_cmds * sizeof(int));
	if (!pids)
		perror("malloc fail");


	if (pipe(fd) == -1)
		perror("pipe fail");

	while (cmd)
	{
		// if there's more than one cmd and we not in the last cmd,
		// create a pipe
		if (cmd->index < cmd_meta.num_cmds - 1 && cmd_meta.num_cmds > 1)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe fail");
				free(pids);
				exit(EXIT_FAILURE);
			}
		}
		pid_t pid = fork();
		if (pid == -1)
		{
			perror("fork fail");
			//some cleanup, close fds, free pids
		}
		else if (pid == 0)
		{
			// maybe fd and prev_pipe_read_fd should be
			// pointers?but it seems to work
			child_process(cmd, prev_pipe_read_fd, fd, cmd_meta.num_cmds);
		}
		else
		{
			//parent process, pushes each child process pid to
			//array, closes the prev pipe for all processes except
			//first, if it not the last closes the read pipe and
			//sets the next pipe
			pids[cmd->index] = pid;
			if (prev_pipe_read_fd != STDIN_FILENO)
				close(prev_pipe_read_fd);
			if (cmd->index < cmd_meta.num_cmds - 1)
			{
				close(fd[1]);
				prev_pipe_read_fd = fd[0];
			}
		}
		cmd = cmd->pipe_next;
	}
	wait_for_children(pids, cmd_meta.num_cmds);
	free(pids);
}
