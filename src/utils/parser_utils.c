#include "../includes/minishell.h"

int is_shell_operator(char *s)
{
	if (!s)
		return (0);
	if (ft_strcmp(s, ">") == 0 || ft_strcmp(s, "<") == 0
		|| ft_strcmp(s, ">>") == 0 || ft_strcmp(s, "<<") == 0
		||	ft_strcmp(s, "|") == 0)
		return (1);
	return (0);
}

t_vars	*init_envp(char **envp)
{
	t_vars	*head;
	t_vars	*new_var;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		new_var = parse_var(envp[i]);
		if (!new_var)
		{
			clean_myenv(head);
			return (NULL);
		}
		push_list(&head, new_var);
		i++;
	}
	return (head);
}
