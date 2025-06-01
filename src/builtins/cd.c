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
	char		pwd[PATH_MAX];
	struct stat	st;
	//can;t figure out norminette here
	getcwd(pwd, PATH_MAX);
	if (command->arguments[1] == NULL)
	{
		update_var_value(find_var(myenv, "OLDPWD"), (const char *)pwd);
		if (chdir(find_var(myenv, "HOME")->value) != 0)
			return (ft_perror(command->name, find_var(myenv, "HOME")->value, 1, NULL));
		update_var_value(find_var(myenv, "PWD"), find_var(myenv, "HOME")->value);
	}
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
			update_var_value(find_var(myenv, "OLDPWD"), (const char *)pwd);
			getcwd(pwd, PATH_MAX);
			update_var_value(find_var(myenv, "PWD"), (const char *)pwd);
		}
	}
	return (0);
}
