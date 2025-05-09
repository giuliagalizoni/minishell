#include "../includes/minishell.h"

void	echo(t_command *cmd)
{
	int	i;
	int	flag;

	flag = 0;
	if (!cmd->arguments[1])
	{
		printf("\n");
		return ;
	}
	i = 1;
	if (is_equal(cmd->arguments[1], "-n"))
	{
		i++;
		flag = 1;
	}
	while (cmd->arguments[i])
	{
		printf("%s", cmd->arguments[i]);
		if (cmd->arguments[i + 1])
			printf(" ");
		i++;
	}
	if (!flag)
		printf("\n");
}
