#include "../includes/minishell.h"

int	cd(t_command *command)
{
	if (command->arguments[1] == NULL)
		return (1);
	else
	{
		if (chdir(command->arguments[1]) != 0)
		{
			/*
			if (access(command->arguments[1], F_OK) != 0)
				return (ft_perror(command, command->arguments[1], 1, 1, "Not a directory"));
			else
				return (ft_perror(command, command->arguments[1], 1, 1, "No such file or directory"));
				*/
			return (ft_perror(command, command->arguments[1], 1, 1, NULL));
		}
	}
	return (0);
}
