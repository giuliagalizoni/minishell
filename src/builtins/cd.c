/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:55:08 by ggalizon          #+#    #+#             */
/*   Updated: 2025/06/02 12:55:10 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	cd_home(t_command *command, char *pwd, t_vars *myenv)
{
	if (find_var(myenv, "HOME"))
	{
		if (find_var(myenv, "OLDPWD"))
			update_var_value(find_var(myenv, "OLDPWD"), (const char *)pwd);
		if (chdir(find_var(myenv, "HOME")->value) != 0)
			return (ft_perror(command->name, \
						find_var(myenv, "HOME")->value, 1, NULL));
		if (find_var(myenv, "PWD"))
			update_var_value(find_var(myenv, "PWD"), \
					find_var(myenv, "HOME")->value);
	}
	else
		return (ft_perror(command->name, NULL, 1, "HOME not set"));
	return (1);
}

int	cd(t_command *command, t_vars *myenv)
{
	char		pwd[PATH_MAX];
	struct stat	st;

	getcwd(pwd, PATH_MAX);
	if (command->arguments[1] == NULL)
		return (cd_home(command, pwd, myenv));
	else if (command->arguments[2])
		return (ft_perror(command->name, NULL, 1, "too many arguments"));
	else
	{
		if (stat(command->arguments[1], &st) == -1)
			return (ft_perror(command->name, command->arguments[1], 1,
					"No such file or directory"));
		if (chdir(command->arguments[1]) != 0)
			return (ft_perror(command->name, command->arguments[1], 1, NULL));
		else
		{
			if (find_var(myenv, "OLDPWD"))
				update_var_value(find_var(myenv, "OLDPWD"), (const char *)pwd);
			getcwd(pwd, PATH_MAX);
			if (find_var(myenv, "PWD"))
				update_var_value(find_var(myenv, "PWD"), (const char *)pwd);
		}
	}
	return (0);
}
