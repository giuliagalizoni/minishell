#include "../includes/minishell.h"

void	exit_shell(t_msh *msh, t_command *command)
{
	int	arg_len;

	if(command != NULL)
	{
		arg_len = array_length((void **)command->arguments);
		if (arg_len > 1 && !is_num_string(command->arguments[1]))
			msh->exit_status = ft_perror(command->name, command->arguments[1], 0, "numeric argument required");
		else if (arg_len > 2)
			msh->exit_status = ft_perror(command->name, NULL, 1, "too many arguments");
		if (arg_len == 2 && is_num_string(command->arguments[1]))
				msh->exit_status = ft_atoi(command->arguments[1]);
		else
			msh->exit_status = 0;
	}
	// Ensure all resources are freed before exiting.
	// The msh->exit_status should be set appropriately before calling this,
	// e.g., by the exit builtin parsing its argument, or by the last command executed.
	clear_command_chain(msh->command);
	clean_myenv(msh->myenv);
	rl_clear_history();

	if (msh->num_cmds < 2)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	exit(msh->exit_status);
}
