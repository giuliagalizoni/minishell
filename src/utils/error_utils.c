#include "../includes/minishell.h"

void	error_cleanup(t_msh *msh, char *error)
{
	// TODO Better print format
	if (error)
		perror(error);
	cleanup(msh);
}
// I'll make this function better soon
int	return_error(char *error_msg)
{
	if (error_msg)
		perror(error_msg);
	return (0);
}

void	exit_process(t_msh *msh, char *error, int exit_code)
{
	error_cleanup(msh, error);
	if (exit_code >= 0)
		exit(exit_code);
}
