#include "includes/minishell.h"

// TODO
// order list by ascii
// deal with syntax errors *= bash: export: `*=': not a valid identifier
// cleanup before exiting
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
		printf("export %s=\"%s\"\n", vars->key, vars->value);
		vars = vars->next;
	}
}

t_vars *parse_var(const char *arg)
{
	t_vars *new_var;
	char *eq;
	char *key;
	char *value;

	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		key = ft_strdup(arg);
		value = NULL;
    }
    else
    {
        key = ft_substr(arg, 0, eq - arg);
        value = ft_strdup(eq + 1);
        // Handle quoted values
        if (value && (value[0] == '\'' || value[0] == '\"') && value[ft_strlen(value) - 1] == value[0])
        {
            char *temp = ft_substr(value, 1, ft_strlen(value) - 2);
            free(value);
            value = temp;
        }
    }

    new_var = malloc(sizeof(t_vars));
    new_var->key = key;
    new_var->value = value;
    new_var->next = NULL;

    return new_var;
}

t_vars *init_envp(char **envp)
{
	t_vars *head = NULL;
	t_vars *new_var;
	int i = 0;

	while (envp[i])
	{
		new_var = parse_var(envp[i]);
		push_list(&head, new_var);
		i++;
	}
	return head;
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

void	export(t_command *cmd, t_vars **exp_vars)
{
	int i;
	t_vars	*new_var;

	if (!cmd->arguments[1])
	{
		print_vars(*exp_vars);
		return ;
	}
	i = 1;
	while (cmd->arguments[i])
	{
		new_var = parse_var(cmd->arguments[i]);
		if (new_var)
		{
			add_or_update_var(exp_vars, new_var->key, new_var->value);
			free(new_var->key);
			free(new_var->value);
			free(new_var);
		}
		i++;
	}
}
