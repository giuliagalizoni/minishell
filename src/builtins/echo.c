#include "../includes/minishell.h"

static int	is_n_flag(char *arg)
{
	int i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while(arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	echo(t_command *cmd)
{
	int	i;
	int	flag;

	flag = 1;
	if (!cmd->arguments[1])
	{
		printf("\n");
		return (0);
	}
	i = 1;
	while (cmd->arguments[i] && is_n_flag(cmd->arguments[i]))
	{
		flag = 0;
		i++;
	}
	while(cmd->arguments[i])
	{
		printf("%s", cmd->arguments[i]);
		if (cmd->arguments[i + 1])
			printf(" ");
		i++;
	}
	if (flag)
		printf("\n");
	return (0);
}
