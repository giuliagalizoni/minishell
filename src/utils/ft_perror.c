#include "../includes/minishell.h"

static char	*create_base_shell_prefix(char *cmd_name, int *name_added)
{
	char	*base;
	char	*tmp;

	*name_added = 0;
	base = ft_strdup("conchinha: ");
	if (!base)
		return (NULL);
	if (cmd_name)
	{
		tmp = ft_strjoin(base, cmd_name);
		free(base);
		if (!tmp)
			return (NULL);
		base = tmp;
		*name_added = 1;
	}
	return (base);
}

static char	*append_err_arg(char *current_str, int name_was_added, char *arg)
{
	char	*new_str;
	char	*tmp;

	if (!arg)
		return (current_str);
	if (name_was_added)
		tmp = ft_triplestrjoin(current_str, ": ", arg);
	else
		tmp = ft_strjoin(current_str, arg);
	free(current_str);
	if (!tmp)
		return (NULL);
	new_str = tmp;
	return (new_str);
}

static void	print_error_and_free(char *full_prefix, char *err_msg)
{
	char	*final_str;

	if (err_msg)
	{
		final_str = ft_triplestrjoin(full_prefix, ": ", err_msg);
		free(full_prefix);
		if (!final_str)
		{
			perror("ft_perror (final_join_print)");
			return ;
		}
		ft_putstr_fd(final_str, STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
		free(final_str);
	}
	else
	{
		perror(full_prefix);
		free(full_prefix);
	}
}

int	ft_perror(char *cmd_name, char *arg, int status, char *err_msg)
{
	char	*base_prefix;
	char	*prefix_with_arg;
	int		name_added_flag;

	base_prefix = create_base_shell_prefix(cmd_name, &name_added_flag);
	if (!base_prefix)
	{
		perror("ft_perror (base)");
		return (status);
	}
	prefix_with_arg = append_err_arg(base_prefix,
			name_added_flag, arg);
	if (!prefix_with_arg)
	{
		perror("ft_perror (arg_append)");
		return (status);
	}
	print_error_and_free(prefix_with_arg, err_msg);
	return (status);
}
