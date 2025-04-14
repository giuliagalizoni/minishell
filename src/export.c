#include "includes/minishell.h"

void	print_arr(char **arr)
{
	int	i;

	i = 0;
	while(arr[i])
	{
		printf("export %s\n", arr[i]);
		i++;
	}
}

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
		printf("key: %s\nvalue: %s\n", vars->key, vars->value);
		vars = vars->next;
	}
}

void	add_or_update_var(t_vars **head, char *key, char *value)
{
	t_vars *current = *head;
	t_vars *new_var;

	while(current)
	{
		if (is_equal(current->key, key))
		{
			free(current->value);
			current->value = ft_strdup(value);
			return;
		}
		current = current->next;
	}
	new_var = malloc(sizeof(t_vars));
	new_var->key = ft_strdup(key);
	new_var->value = ft_strdup(value);
	push_list(head, new_var);
}

void	export(t_command *cmd, char **envp, t_vars **exp_vars)
{
	// char	**envp_copy;
	char	**temp;
	int i;

	if (!cmd->arguments[1])
		print_arr(envp);
	// envp_copy = envp;
	i = 1;
	while(cmd->arguments[i])
	{
		temp = ft_split(cmd->arguments[i], '=');
		if (!temp[0] || !temp[i])
		{
			free_arr((void **)temp);
			i++;
			continue;
		}
		add_or_update_var(exp_vars, temp[0], temp[1]);
		free_arr((void **)temp);
		i++;
	}
	print_vars(*exp_vars);
}

	// key=value
	// if key -> update
	// if !key -> add key and value
	// if !value -> keep key empty
	// add key to envp

	//export alone should show all variables

	//syntax errors
