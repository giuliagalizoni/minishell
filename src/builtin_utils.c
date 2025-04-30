#include "includes/minishell.h"

int	is_builtin(char *name)
{
	if (is_equal(name, "echo") || is_equal(name, "cd") || is_equal(name, "pwd"))
		return (1);
	if (is_equal(name, "export") || is_equal(name, "unset") || is_equal(name, "env"))
		return (1);
	if (is_equal(name, "exit") || is_equal(name, "cd") || is_equal(name, "pwd"))
		return (1);
	return (0);
}

int	builtin_router(t_msh *msh)
{
	int	status;

	status = 0;
	if (is_equal(msh->command->name, "exit"))
		exit_shell(msh);
	else if (is_equal(msh->command->name, "echo"))
		echo(msh->command);
	else if (is_equal(msh->command->name, "export"))
		export(msh);
	else if (is_equal(msh->command->name, "env"))
		print_env(msh->myenv);
	else if (is_equal(msh->command->name, "cd"))
		status = cd(msh->command);
	else if (is_equal(msh->command->name, "pwd"))
		status = pwd();
	else if (is_equal(msh->command->name, "unset"))
		unset(msh);
	// TODO need to change builtins to send me their exit codes
	return (status);
}

void	child_builtin(t_msh *msh)
{
	int	status;

	status = builtin_router(msh);
	exit(status);
}
