#include "../includes/minishell.h"

void	init_value(char **key, char **value, const char *arg, char *eq)
{
	char	*temp;
	char	*content;

	*key = ft_substr(arg, 0, eq - arg);
	content = ft_strdup(eq + 1);
	if (content && (content[0] == '\'' || content[0] == '\"')
		&& content[ft_strlen(content) - 1] == content[0])
	{
		temp = ft_substr(content, 1, ft_strlen(content) - 2);
		free(content);
		content = temp;
	}
	*value = content;
}

t_vars	*parse_var(char *arg)
{
	t_vars	*new_var;
	char	*eq;
	char	*key;
	char	*value;

	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		key = ft_strdup(arg);
		value = NULL;
	}
	else
		init_value(&key, &value, arg, eq);
	if (validate_key(key))
	{
		export_error(arg);
		return (NULL);
	}
	new_var = malloc(sizeof(t_vars));
	if (!new_var)
		return (handle_malloc_error(&key, &value, "malloc failed"));
	new_var->key = key;
	new_var->value = value;
	new_var->next = NULL;
	return (new_var);
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

int	update_var_value(t_vars *var, const char *value)
{
	free(var->value);
	if (value)
	{
		var->value = ft_strdup(value);
		if (!var->value)
		return (0);
	}
	else
		var->value = NULL;
	return (1);
}

int	add_or_update_var(t_vars **head, char *key, char *value)
{
	t_vars	*current;
	t_vars	*new_var;

	current = *head;
	while (current)
	{
		if (is_equal(current->key, key))
			return (update_var_value(current, value));
		current = current->next;
	}
	new_var = malloc(sizeof(t_vars));
	if (!new_var)
		return (ft_perror(NULL, NULL, 0, "malloc failed"));
	new_var->key = ft_strdup(key);
	if (!new_var->key)
		return (free(new_var), ft_perror(NULL, NULL, 0, "malloc failed"));
	if (value)
		new_var->value = ft_strdup(value);
	else
		new_var->value = NULL;
	new_var->next = NULL;
	return (push_list(head, new_var), 1);
}

int	export(t_msh *msh, t_command *command)
{
	int		i;
	t_vars	*new_var;

	if (!command->arguments[1])
		return (sort_vars_list(msh->myenv), print_vars(msh->myenv), 0);
	i = 1;
	while (command->arguments[i])
	{
		new_var = parse_var(command->arguments[i]);
		if (new_var)
		{
			if (!add_or_update_var(&msh->myenv, new_var->key, new_var->value))
				return (1);
			free(new_var->key);
			free(new_var->value);
			free(new_var);
		}
		else
			return (1);
		i++;
	}
	return (0);
}
