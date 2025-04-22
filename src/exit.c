#include "includes/minishell.h"

void	exit_shell(t_msh *msh)
{
	clear_command_chain(msh->command);
	clean_myenv(msh->myenv);
	printf("exit\n");
	exit(EXIT_SUCCESS);
}
