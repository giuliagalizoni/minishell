#include "../includes/minishell.h"

void	error_cleanup(t_msh *msh, char *error)
{
	// TODO Better print format
	if (error)
		perror(error);
	cleanup(msh);
}

void	exit_process(t_msh *msh, char *error, int exit_code)
{
	error_cleanup(msh, error);
	if (exit_code >= 0)
		exit(exit_code);
}
