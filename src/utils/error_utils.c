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

void	command_path_error(t_msh *msh)
{
	// TODO maybe some logic for checking if its a folder or not executable
	perror("cmd not found");
	rl_clear_history();
	clean_myenv(msh->myenv);
	clear_command_chain(msh->command);
	// TODO close fds
	exit(127);
}	
