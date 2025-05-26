/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:45:25 by ggalizon          #+#    #+#             */
/*   Updated: 2025/05/26 16:45:26 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		return (ft_perror(command, command->arguments[1], 127, 0, NULL));
	print_env(msh->myenv);
	return (0);
}
