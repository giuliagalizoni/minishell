#include "../includes/minishell.h"

void	cleanup_on_error(t_msh *msh, char *error, int exit_code)
{
	if (error)
		perror(error);
	if (msh->command)
		clear_command_chain(msh->command);
	if (exit_code)
		exit(exit_code);
	// TODO close fds?
}
