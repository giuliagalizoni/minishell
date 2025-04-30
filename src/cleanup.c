#include "includes/minishell.h"

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
