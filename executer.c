#include "includes/minishell.h"
void	process(t_command *cmd, int command_count)
{
	int	fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int	status1;
	int	status2;

	(void)command_count;

	if (pipe(fd) == -1)
		perror("pipe fail");

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
