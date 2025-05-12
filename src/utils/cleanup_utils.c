#include "../includes/minishell.h"

void	clean_myenv(t_vars *myenv)
{
	t_vars	*tmp;

	while (myenv)
	{
		tmp = myenv->next;
		if (myenv->key)
			free(myenv->key);
		if (myenv->value)
			free(myenv->value);
		free(myenv);
		myenv = tmp;
	}
}

//TODO WIP
int	teardown(t_msh *msh, char *error_msg)
{
	clear_command_chain(msh->command);
	if (error_msg)
		perror(error_msg);
	return (1);
}
