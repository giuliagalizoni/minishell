#include "includes/minishell.h"

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

void	builtin_router(t_command *cmd, char **envp)
{
	if (is_equal(cmd->name, "exit"))
		exit_shell(cmd);
	else if (is_equal(cmd->name, "echo"))
		echo(cmd);
	else if (is_equal(cmd->name, "export"))
		export(cmd, envp);
}
