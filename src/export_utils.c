#include "includes/minishell.h"

t_vars	*find_last(t_vars *lst)
{
	if (!lst)
		return (NULL);
	while (lst)
	{
		if (lst->next == NULL)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

void	push_list(t_vars **exp_vars, t_vars *new)
{
	t_vars	*last;

	last = find_last(*exp_vars);
	if (last == NULL)
		*exp_vars = new;
	else
		last->next = new;
}

void	print_vars(t_vars *vars)
{
	while (vars)
	{
		if (!vars->key)
			return ;
		if (vars->value)
			printf("export %s=\"%s\"\n", vars->key, vars->value);
		else
			printf("export %s\n", vars->key);
		vars = vars->next;
	}
}

void	*handle_malloc_error(char **key, char **value, char *str)
{
	if (key)
		free(key);
	if (value)
		free(value);
	perror(str);
	return (NULL);
}

int	validate_key(char	*key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[i]) && !(key[i] == '_'))
		return (1);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && !(key[i] == '_'))
			return (1);
		i++;
	}
	return (0);
}
