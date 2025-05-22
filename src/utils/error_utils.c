#include "../includes/minishell.h"

void	error_cleanup(t_msh *msh, char *error)
{
	// TODO Better print format
	if (error)
		perror(error);
	cleanup(msh);
}
// I'll make this function better soon
int	return_error(char *error_msg)
{
	if (error_msg)
		perror(error_msg);
	return (0);
}

void	exit_process(t_msh *msh, char *error, int exit_code)
{
	error_cleanup(msh, error);
	if (exit_code >= 0)
		exit(exit_code);
}

int	ft_perror(t_command *command, char *arg, int	status)
{
	char	*prefix;
	char	*cmd_str;
	char	*err_message;

	prefix = "conchinha: ";
	cmd_str = ft_strjoin(command->name, ": ");
	err_message = ft_triplestrjoin(prefix, cmd_str, arg);
	perror(err_message);
	free(cmd_str);
	free(err_message);
	return (status);
}
