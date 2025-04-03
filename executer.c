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

void	process(t_command *cmd)
{
	while (cmd)
	{
		child_process(cmd);
		cmd = cmd->next;
	}
}

void	child_process(t_command *cmd)
{
	pid_t	pid;
	int	fd[2];
	int	cmd_count;
	
	// we need to
	// check that it's the first or last cmd in the pipe
	// otherwise we redirect it's stdin to the read end of the last cmd
	// and or the 
	cmd_count = count_commands(cmd);
	printf("cmd count: %d\n", cmd_count);
	if (cmd->index == (cmd_count - 1))
		printf("last command:\n");

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
