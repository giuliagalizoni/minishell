#include "../includes/minishell.h"

int	cd(t_command *command)
{
	if (command->arguments[1] == NULL)
		return (1);
	else
	{
		if (chdir(command->arguments[1]) != 0)
		{
			perror("shell: cd");
			return (1);
		}
	}
	return (0);
}
