#include "includes/minishell.h"

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
