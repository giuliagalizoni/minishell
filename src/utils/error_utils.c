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
		ft_perror(command, arg, status, err_msg);
	cleanup(msh);
	exit(status);
}

int	ft_perror(t_command *command, char *arg, int status, char *err_msg)
{
	char	*tmp;
	char	*err_str;

	err_str = ft_strdup("conchinha: ");
	if (command->name)
	{
		tmp = ft_strjoin(err_str, command->name);
		free(err_str);
		err_str = tmp;
	}
	if (arg && !command->name)
	{
		tmp = ft_strjoin(err_str, arg);
		free(err_str);
		err_str = tmp;
	}
	else
	{
		if (arg)
		{
			tmp = ft_triplestrjoin(err_str, ": ", arg);
			free(err_str);
			err_str = tmp;
		}
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

int	invalid_option_error(t_command *cmd, char* arg, int status)
{
	ft_putstr_fd("conchinha: ", STDERR_FILENO);
	ft_putstr_fd(cmd->name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": invalid option\n", STDERR_FILENO);
	return (status);
}
