#include "../includes/minishell.h"

void	find_and_delete(t_vars **head, char *key)
{
	t_vars	*current;
	t_vars	*prev;

	current = *head;
	prev = NULL;
	while (current)
	{
		if (is_equal(current->key, key))
		{
			if (prev == NULL)
				*head = current->next;
			else
				prev->next = current->next;
			free(current->key);
			if (current->value)
				free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}


int	unset(t_msh *msh, t_command *command)
{
	int	i;

	if (!command->arguments[1])
		return (0);
	if (command->arguments[1][0] == '-')
		return invalid_option_error(command, command->arguments[1], 2);
	i = 1;
	while (msh->command->arguments[i])
	{
		find_and_delete(&msh->myenv, command->arguments[i]);
		i++;
	}
	return (0);
}
