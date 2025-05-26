/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:37:48 by ggalizon          #+#    #+#             */
/*   Updated: 2025/05/26 16:37:49 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
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
	char	*inner_content;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') || \
					(str[0] == '\'' && str[len - 1] == '\'')))
	{
		inner_content = ft_substr(str, 1, len - 2);
		if (!inner_content)
			return (ft_strdup(str));
		if (is_shell_operator(inner_content))
		{
			return (inner_content);
		}
		else
		{
			free(inner_content);
			return (ft_strdup(str));
		}
	}
	return (ft_strdup(str));
}

static void	print_arguments(t_command *cmd, int i)
{
	char	*arg;

	arg = trim_quotes(cmd->arguments[i]);
	printf("%s", arg);
	free(arg);
	if (cmd->arguments[i + 1])
		printf(" ");
}

int	echo(t_command *cmd)
{
	int		i;
	int		flag;

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
	while (cmd->arguments[i])
	{
		print_arguments(cmd, i);
		i++;
	}
	if (flag)
		printf("\n");
	return (0);
}
