#include "includes/minishell.h"

void	ft_cmdclear(t_command **cmd, void (*del)(void *))
{
	t_command	*tmp;

	while (*cmd)
	{
		tmp = (*cmd)->next;
		ft_cmddelone(*cmd, del);
		*cmd = tmp;
	}
	free(*cmd);
	*cmd = NULL;
}
