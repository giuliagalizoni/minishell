#include "includes/minishell.h"

void	process(t_command *cmd)
{
	pid_t	pid;
	pid = fork();
	if (pid == 0)
	{
//		cmd = get_cmd_path(args[0], get_rawpath(envp)
		execve(cmd->name, cmd->arguments, NULL);
	}
//	if (cmd)
//		execve(cmd, &args[0], envp);
	perror("Command not found");
}
