#include "../includes/minishell.h"

t_vars	*find_var(t_vars *var, char *key)
{
	while (var)
	{
		if (is_equal(var->key, key))
			return (var);
		var = var->next;
	}
	return (NULL);
}
int	cd(t_command *command, t_vars *myenv)
{
	char	pwd[PATH_MAX];

	getcwd(pwd, PATH_MAX);
	if (command->arguments[1] == NULL)
		return (ft_perror(command, NULL, 1, 1, "Usage: cd <directory>"));
	else if (command->arguments[2])
		return (ft_perror(command, NULL, 1, 1, "too many arguments"));
	else
	{
		/*
		if (stat(command->arguments[1]) == -1)
		{
			if (errno == ENOENT)
				return (ft_perror(command, command->arguments[1], 1, 1, "No such file or directory"));
			else
				return (ft_perror(command, command->arguments[1], 1, 1, NULL));
		}
		*/
					
		if (chdir(command->arguments[1]) != 0)
			return (ft_perror(command, command->arguments[1], 1, 1, NULL));
		else
		{
			update_var_value(find_var(myenv, "OLDPWD"), (const char *)pwd);
			getcwd(pwd, PATH_MAX);
			update_var_value(find_var(myenv, "PWD"), (const char *)pwd);
		}

	}
	return (0);
}
