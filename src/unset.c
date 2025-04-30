#include "includes/minishell.h"

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

	// TODO: Set exit status?
void	unset(t_msh *msh)
{
	int	i;

	if (!msh->command->arguments[1])
		return ;
	i = 1;
	while (msh->command->arguments[i])
	{
		find_and_delete(&msh->myenv, msh->command->arguments[i]);
		i++;
	}
}
