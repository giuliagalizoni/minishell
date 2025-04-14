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
	int i;
	char *arg;
	char *eq;
	char *key;
	char *value;

	if (!cmd->arguments[1])
	{
		print_arr(envp);
		return ;
	}
	i = 1;
	while(cmd->arguments[i])
	{
		arg = cmd->arguments[i];
		eq = ft_strchr(arg, '=');
		if (!eq)
			add_or_update_var(exp_vars, arg, NULL);
		else
		{
			key = ft_substr(arg, 0, eq - arg);
			value = ft_strdup(eq+1);
			if ((value[0] == '\'' || value[0] == '\"') && value[ft_strlen(value) - 1] == value[0])
			{
				char *temp = ft_substr(value, 1, ft_strlen(value) - 2);
				free(value);
				value = temp;
			}
			add_or_update_var(exp_vars, key, value);
			free(key);
			free(value);
		}
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
