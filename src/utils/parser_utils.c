#include "../includes/minishell.h"

int is_shell_operator(char *s)
{
	if (!s)
		return (0);
	if (ft_strcmp(s, ">") == 0 || ft_strcmp(s, "<") == 0 ||
		ft_strcmp(s, ">>") == 0 || ft_strcmp(s, "<<") == 0 ||
		ft_strcmp(s, "|") == 0)
		return (1);
	return (0);
}
