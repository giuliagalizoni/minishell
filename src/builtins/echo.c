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

char	*trim_quotes(char *str)
{
	size_t	len;
	char	*trimmed;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len - 1] == '\''))
	{
		trimmed = ft_substr(str, 1, len - 2);
		return (trimmed);
	}
	return (ft_strdup(str));
}

int	echo(t_command *cmd)
{
	int	i;
	int	flag;
	char *arg;

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
		arg = trim_quotes(cmd->arguments[i]);
		printf("%s", arg);
		free(arg);
		if (cmd->arguments[i + 1])
			printf(" ");
		i++;
	}
	if (flag)
		printf("\n");
	return (0);
}
