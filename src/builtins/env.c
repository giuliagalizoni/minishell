/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:45:25 by ggalizon          #+#    #+#             */
/*   Updated: 2025/05/27 11:43:17 by ggalizon         ###   ########.fr       */
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

int	env_error(char *arg)
{
	write(2, "env: '", 6);
	write(2, arg, ft_strlen(arg));
	write(2, "': No such file or directory\n", 29);
	return (127);
}

int	env(t_msh *msh, t_command *command)
{
	if (command->arguments[1])
		return (env_error(command->arguments[1]));
	print_env(msh->myenv);
	return (0);
}
