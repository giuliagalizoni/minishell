#include "includes/minishell.h"

void	exit_shell(t_msh *msh)
{
	clear_command_chain(msh->command);
	clean_myenv(msh->myenv);
	clear_history();
	printf("byeeeeeee\n");
	exit(EXIT_SUCCESS);
}
