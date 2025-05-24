#include "../includes/minishell.h"

void	error_cleanup(t_msh *msh)
{
	cleanup(msh);
}
// I'll make this function better soon
int	return_error(char *error_msg)
{
	if (error_msg)
		perror(error_msg);
	return (0);
}

void	exit_process(t_msh *msh, t_command *command, char *arg, char *err_msg, int status)
{
	if (status != 0)
		ft_perror(command, arg, status, 1, err_msg);
	cleanup(msh);
	exit(status);
}

int	ft_perror(t_command *command, char *arg, int status, int has_prefix, char *err_msg)
{
	char	*tmp;
	char	*err_str;

	if (has_prefix)
		err_str = ft_strdup("conchinha: ");
	else
		err_str = ft_strdup("");
	if (command->name)
	{
		tmp = ft_strjoin(err_str, command->name);
		free(err_str);
		err_str = tmp;
	}
	if (arg)
	{
		tmp = ft_triplestrjoin(err_str, ": ", arg);
		free(err_str);
		err_str = tmp;
	}
	if (err_msg)
	{
		tmp = ft_triplestrjoin(err_str, ": ", err_msg);
		free(err_str);
		err_str = tmp;
		ft_putstr_fd(err_str, STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
		free(err_str);
		return (status);
	}
	perror(err_str);
	free(err_str);
	return (status);
}
