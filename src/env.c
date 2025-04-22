#include "includes/minishell.h"

void	print_env(t_vars *myenv)
{
	while (myenv)
	{
		if (!myenv->key)
			return ;
		if (myenv->value)
			printf("%s=\"%s\"\n", myenv->key, myenv->value);
		myenv = myenv->next;
	}
}
