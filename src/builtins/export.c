#include "../includes/minishell.h"

void	init_value(char **key, char **value, const char *arg, char *eq)
{
	char	*temp;
	char	*content;

	*key = ft_substr(arg, 0, eq - arg);
	if (!*key)
		return;
	content = ft_strdup(eq + 1);
	if (!content)
	{
		free(*key);
		*key = NULL;
		return;
	}
	if (content && (content[0] == '\'' || content[0] == '\"')
		&& content[ft_strlen(content) - 1] == content[0])
	{
		temp = ft_substr(content, 1, ft_strlen(content) - 2);
		free(content);
		if (!temp)
		{
			free(*key);
			*key = NULL;
			return;
		}
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
		if (!key)
			return (NULL);
		value = NULL;
	}
	else
		init_value(&key, &value, arg, eq);
	if (!key)
		return (NULL);
	if (validate_key(key))
	{
		printf("conchinha: export: `%s\': not a valid identifier\n", arg);
		free(key);
		if (value)
			free(value);
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

void	add_or_update_var(t_vars **head, char *key, char *value)
{
	t_vars	*current;
	t_vars	*new_var;
	char	*new_key;
	char	*new_value;

	current = *head;
	while (current)
	{
		if (is_equal(current->key, key))
		{
			free(current->value);
			if (value)
			{
				new_value = ft_strdup(value);
				if (!new_value)
					return;
				current->value = new_value;
			}
			else
				current->value = NULL;
			return;
		}
		current = current->next;
	}
	new_var = malloc(sizeof(t_vars));
	if (!new_var)
		return;
	new_key = ft_strdup(key);
	if (!new_key)
	{
		free(new_var);
		return;
	}
	if (value)
	{
		new_value = ft_strdup(value);
		if (!new_value)
		{
			free(new_key);
			free(new_var);
			return;
		}
	}
	else
		new_value = NULL;
	new_var->key = new_key;
	new_var->value = new_value;
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
