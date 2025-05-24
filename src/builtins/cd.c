#include "../includes/minishell.h"

int	cd(t_command *command)
{
	if (command->arguments[1] == NULL)
		return (1);
	else if (command->arguments[2])
		return (ft_perror(command, NULL, 1, 1, "too many arguments"));
	else
	{
		if (chdir(command->arguments[1]) != 0)
			return (ft_perror(command, command->arguments[1], 1, 1, NULL));
	}
	return (0);
}
