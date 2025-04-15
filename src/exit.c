#include "includes/minishell.h"

void	exit_shell(t_command *command)
{
	clear_command_chain(command);
	printf("exit\n");
	exit(EXIT_SUCCESS);
}
