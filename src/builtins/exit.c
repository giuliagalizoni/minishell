#include "../includes/minishell.h"

void	exit_shell(t_msh *msh)
{
	// TODO this is shabby
	clear_command_chain(msh->command);
	clean_myenv(msh->myenv);
	rl_clear_history();
//	printf("byeeeeeee\n");
	exit(EXIT_SUCCESS);
}
