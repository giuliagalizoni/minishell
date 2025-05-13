#include "../includes/minishell.h"

int	cleanup_on_error(t_msh *msh, char *error, int exit)
{
	if (error)
		perror(error);
	if (msh->command)
		clear_command_chain(msh->command);
	if (exit)
		exit(EXIT_FAILURE);
	return (0);	
}
