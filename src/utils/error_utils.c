#include "../includes/minishell.h"

void	cleanup_on_error(t_msh *msh, char *error)
{
	if (error)
		perror(error);
	if (msh->command)
		clear_command_chain(msh->command);
	//TODO if exit...exit(??);
}
