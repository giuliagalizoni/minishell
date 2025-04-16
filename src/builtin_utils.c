#include "includes/minishell.h"

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

void	builtin_router(t_msh *msh)
{
	if (is_equal(msh->command->name, "exit"))
		exit_shell(msh->command);
	else if (is_equal(msh->command->name, "echo"))
		echo(msh->command);
	else if (is_equal(msh->command->name, "export"))
		export(msh);
}
