#include "includes/minishell.h"

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
*/

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
	printf("cmd index: %d cmd count: %d\n", cmd->index, cmd_count);
	if (cmd->index == 0)
		pintf("first cmd\n");
	if (cmd->index == (cmd_count - 1))
		printf("index: %d count: %d last command:\n", cmd->index, cmd_count);

	pid = fork();
	if (pid == 0)
	{
		execve(cmd->name, cmd->arguments, NULL);
	}
	wait(NULL);




			/*
	if (pipe(fd) == -1)
		perror("Pipe creation error");// wrapper fn for cleanup?
	pid = fork();
	if (pid == -1)
		perror("Fork creation error");
	if (pid == -1)
	*/
}
