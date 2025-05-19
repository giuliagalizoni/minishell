#include "../includes/minishell.h"

void	exit_shell(t_msh *msh)
{
	// Ensure all resources are freed before exiting.
	// The msh->exit_status should be set appropriately before calling this,
	// e.g., by the exit builtin parsing its argument, or by the last command executed.
	clear_command_chain(msh->command);
	clean_myenv(msh->myenv);
	rl_clear_history();
	// printf("byeeeeeee conchinha exit status: %d\n", msh->exit_status);
	exit(msh->exit_status);
}
