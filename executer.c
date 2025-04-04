#include "includes/minishell.h"
// TODO
// split the function into process / child, see what needs to be passed around
// put the pids in the command struct
// flesh out error handling a bit more (free the command object at every
// error?)
// see if we can get rid of the prev_pipe_read_fd, i don't like it
void	process(t_command *cmd, int command_count)
{
	int	fd[2];
	//TODO move the pids to the cmd stuct
	pid_t	*pids;
	int	status;
	int	prev_pipe_read_fd;


	printf("real num of cmds: %d\n", cmd->cmd_meta.num_cmds);
	prev_pipe_read_fd = STDIN_FILENO;
	pids = malloc(command_count * sizeof(int));
	if (!pids)
		perror("malloc fail");


	if (pipe(fd) == -1)
		perror("pipe fail");

	while (cmd)
	{
		if (cmd->index < command_count - 1 && command_count > 1)
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
			// TODO do i really need this prev_pipe_read_fd
			if (prev_pipe_read_fd != STDIN_FILENO) 
			{
				if (dup2(prev_pipe_read_fd, STDIN_FILENO) == -1)
				{
					perror("dup2 fail for stdin redirection");
					exit(EXIT_FAILURE);
				}
				close(prev_pipe_read_fd);
			}
			if (cmd->index < command_count - 1)
			{
				close(fd[0]);
				if (dup2(fd[1], STDOUT_FILENO) == -1)
				{
					perror("dup2 failed for stdout redirection");
					exit(EXIT_FAILURE);
				}
				close(fd[1]);
			}
			execve(cmd->name, cmd->arguments, NULL);
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
		else
		{
			pids[cmd->index] = pid;
			if (prev_pipe_read_fd != STDIN_FILENO)
				close(prev_pipe_read_fd);
			if (cmd->index < command_count - 1)
			{
				close(fd[1]);
				prev_pipe_read_fd = fd[0];
			}
		}
		cmd = cmd->pipe_next;
	}
	int	i;
	i = 0;
	while (i < command_count)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
	free(pids);


	/*
	pid1 = fork();
	if (pid1 == -1)
		perror("fork fail");
	if (pid1 == 0)
	{
		close(fd[0]);
		if (dup2(fd[1], 1) == -1)
		{
			perror("dup2 fail");
			close(fd[1]);
		}
		close(fd[1]);
		execve(cmd->name, cmd->arguments, NULL);
		perror("exec fail");
	}
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork 2 fail");
		close(fd[0]);
		close(fd[1]);
		waitpid(pid1, &status1, 0);
		exit(1);
	}
	if (pid2 == 0)
	{
		close(fd[1]);
		if (dup2(fd[0], 0) == -1)
		{
			perror("dup2 fail");
			close(fd[0]);
			exit(1);
		}
		close(fd[0]);
		execve(cmd->pipe_next->name, cmd->pipe_next->arguments, NULL);
		perror("exec fail");
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	*/

}
/*
void	process(t_command *cmd)
{
	pid_t	pid;
	pid = fork();
	if (pid == 0)
	{
		execve(cmd->name, cmd->arguments, NULL);
	}
	wait(NULL);
}

void	process(t_command *cmd, int cmd_count)
{
	while (cmd)
	{
		child_process(cmd, cmd_count);
		cmd = cmd->pipe_next;
	}
}

void	child_process(t_command *cmd, int cmd_count)
{
	pid_t	pid;
	int	fd[2];
	// we need to
	// check that it's the first or last cmd in the pipe
	// otherwise we redirect it's stdin to the read end of the last cmd
	// and or the 
	if (pipe(fd) == -1)
		perror("Pipe creation error");// wrapper fn for cleanup?
	pid = fork();
	if (pid == -1)
		perror("Fork creation error");
	if (pid == 0)
	{
		printf("pid==0first cmd: %s index %d is_pipe? %d\n", cmd->name, cmd->index, cmd->is_pipe);
		if (cmd->index == 0 && !(cmd->is_pipe))
			execve(cmd->name, cmd->arguments, NULL);
		if (cmd->index == 0 && cmd->is_pipe)
		{
			printf("first cmd\n");
			dup2(fd[1], 1);
			close(fd[1]);
			execve(cmd->name, cmd->arguments, NULL);
		}
		if (cmd->index > 0 && cmd->index < (cmd_count - 1))
			printf("Mid cmd\n");
		if (cmd->index == cmd_count - 1 )
		{

			close(fd[1]);
			dup2(fd[0], 0);
			close(fd[0]);
			printf("last cmd\n");
			execve(cmd->name, cmd->arguments, NULL);
		}
	}
	wait(NULL);




	if (pipe(fd) == -1)
		perror("Pipe creation error");// wrapper fn for cleanup?
	pid = fork();
	if (pid == -1)
		perror("Fork creation error");
	if (pid == -1)
}
*/
