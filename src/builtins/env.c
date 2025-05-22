#include "../includes/minishell.h"

static int	print_env(t_vars *myenv)
{
	while (myenv)
	{
		if (!myenv->key)
			return (0);
		if (myenv->value)
			printf("%s=\"%s\"\n", myenv->key, myenv->value);
		myenv = myenv->next;
	}
	return (0);
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

int	env(t_msh *msh, t_command *command)
{
	if (command->arguments[1])
	{
		// apply new perror after
		perror("env: ");
		return (127);
	}
	print_env(msh->myenv);
	return (0);
}
