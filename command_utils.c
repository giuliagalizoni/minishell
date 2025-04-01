#include "includes/minishell.h"

void	ft_cmdclear(void *arg)
{
	t_command	*cmd;
	cmd = (t_command *)arg;
	free(cmd->name);
	free_arr(cmd->arguments);
}

void	ft_cmddelone(t_command *cmd, void (*del)(void *))
{
	del(cmd);
	free(cmd);
}

void	ft_cmdlstclear(t_command **cmd, void (*del)(void *))
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
