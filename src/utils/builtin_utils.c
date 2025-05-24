#include "../includes/minishell.h"

int	is_builtin(char *name)
{
	if (!name)
		return (0);
	if (is_equal(name, "echo") || is_equal(name, "cd") || is_equal(name, "pwd"))
		return (1);
	if (is_equal(name, "export") || is_equal(name, "unset")
		|| is_equal(name, "env"))
		return (1);
	if (is_equal(name, "exit") || is_equal(name, "cd") || is_equal(name, "pwd"))
		return (1);
	return (0);
}

int	builtin_router(t_msh *msh, t_command *command)
{
	int	status;

	status = 0;
	if (is_equal(command->name, "exit"))
		exit_shell(msh);
	else if (is_equal(command->name, "echo"))
		status = echo(command);
	else if (is_equal(command->name, "export"))
		status = export(msh, command);
	else if (is_equal(command->name, "env"))
		status = env(msh, command);
	else if (is_equal(command->name, "cd"))
		status = cd(command, msh->myenv);
	else if (is_equal(command->name, "pwd"))
		status = pwd();
	else if (is_equal(command->name, "unset"))
		status = unset(msh, command);
	msh->exit_status = status;
	return (status);
}

void	child_builtin(t_msh *msh, t_command *command)
{
	msh->exit_status = builtin_router(msh, command);
	exit_process(msh, command, NULL, NULL, msh->exit_status);
}
