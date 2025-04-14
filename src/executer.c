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

static void	output_redirection(t_command *cmd)
{
	int	i;
	int	file;

	i = 0;
	while(cmd->output_redirect[i])
	{
		file = open(cmd->output_redirect[i], O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (file == -1)
			perror("Bad file descriptor");// cleanup routine here
		dup2(file, 1);
		close(file);
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
	if (cmd->input_redirect)
		input_redirection(cmd);
	if (cmd->output_redirect)
		output_redirection(cmd);
	else
	{
		execve(cmd->path, cmd->arguments, NULL);
		perror("execve failed"); //move this here so the error wont be printed when it's builtin
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS); //put this here to exit the child process when builtin, maybe we can move it to the router later
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

void	process(t_command *cmd, int num_cmds, t_vars **exp_vars)
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
		//TODO do we need a case for a single command?
		//TODO what to do when its more than one cmd?bash seems to just
		//eat it up
		if (is_builtin(cmd->name))
		{
			// TODO what are we doing with the pipes and everything
			// here
			builtin_router(cmd, exp_vars);
			cmd = cmd->pipe_next;
			// include parent process cleanup here?
		}
		else
		{
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
	}
	wait_for_children(pids, num_cmds);
	free(pids);
}
