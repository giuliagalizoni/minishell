#include "includes/minishell.h"

void	exit_shell(t_command *cmd)
{
	// TODO not exiting because we are in a forked process
	// need to move the check for exit to the parent??
	printf("poopi\n");
	printf("cmd %s\n", cmd->name);
	exit(1);
	printf("why no exiting\n");
}

int	is_equal(char *name, char *builtin)
{
	if (ft_strlen(name) != ft_strlen(builtin))
		return 0;
	if (!ft_strncmp(name, builtin, ft_strlen(builtin)))
		return (1);
	return (0);

}
int	is_builtin(char *name)
{
	if (is_equal(name, "echo") || is_equal(name, "cd") || is_equal(name, "pwd"))
		return (1);
	if (is_equal(name, "export") || is_equal(name, "unset") || is_equal(name, "env"))
		return (1);
	if (is_equal(name, "exit"))
		return (1);
	return (0);
}

void	builtin_router(t_command *cmd)
{
	if (is_equal(cmd->name, "exit"))
		exit_shell(cmd);
}
