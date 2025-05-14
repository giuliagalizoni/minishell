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

t_vars	*parse_var(const char *arg)
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
		printf("conchinha: export: `%s\': not a valid identifier\n", arg);
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
		if (new_var)
			push_list(&head, new_var);
		i++;
	}
	return (head);
}
// TODO: Check ft_strdup results
void	add_or_update_var(t_vars **head, char *key, char *value)
{
	t_vars	*current;
	t_vars	*new_var;

	current = *head;
	while (current)
	{
		if (is_equal(current->key, key))
		{
			free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			return ;
		}
		current = current->next;
	}
	new_var = malloc(sizeof(t_vars));
	new_var->key = ft_strdup(key);
	if (value)
		new_var->value = ft_strdup(value);
	else
		new_var->value = NULL;
	new_var->next = NULL;
	push_list(head, new_var);
}

void	export(t_msh *msh)
{
	int		i;
	t_vars	*new_var;

	if (!msh->command->arguments[1])
	{
		sort_vars_list(msh->myenv);
		print_vars(msh->myenv);
		return ;
	}
	i = 1;
	while (msh->command->arguments[i])
	{
		new_var = parse_var(msh->command->arguments[i]);
		if (new_var)
		{
			add_or_update_var(&msh->myenv, new_var->key, new_var->value);
			free(new_var->key);
			free(new_var->value);
			free(new_var);
		}
		i++;
	}
}
