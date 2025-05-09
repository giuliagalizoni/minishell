#include "../includes/minishell.h"

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

char	*get_var_value(t_vars *head, char *key)
{
	t_vars	*current;

	current = head;
	while (current)
	{
		if (is_equal(current->key, key))
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
