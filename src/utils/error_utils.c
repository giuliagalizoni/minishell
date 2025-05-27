#include "../includes/minishell.h"

void	error_cleanup(t_msh *msh)
{
	cleanup(msh);
}

void	export_error(char *arg)
{
	write(2, "conchinha: export: `", 21);
	write(2, arg, ft_strlen(arg));
	write(2, "\': not a valid identifier\n", 26);
}

void	exit_process(t_msh *msh, t_command *command, char *arg,
			char *err_msg, int status)
{
	if (status != 0)
		ft_perror(command->name, arg, status, err_msg);
	cleanup(msh);
	exit(status);
}

int	invalid_option_error(t_command *cmd, char *arg, int status)
{
	ft_putstr_fd("conchinha: ", STDERR_FILENO);
	ft_putstr_fd(cmd->name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": invalid option\n", STDERR_FILENO);
	return (status);
}
