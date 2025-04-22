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
int	validate_key(char	*key)
{
	int i;

	i = 0;
	if (!ft_isalpha(key[i]) && !(key[i] == '_'))
		return 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && !(key[i] == '_'))
			return 1;
		i++;
	}
	return 0;
}

void	init_value(char **key, char **value, const char *arg, char *eq)
{
	char *temp;

	*key = ft_substr(arg, 0, eq - arg);
	*value = ft_strdup(eq + 1);
	if (*value && (*value[0] == '\'' || *value[0] == '\"') && *value[ft_strlen(*value) - 1] == *value[0])
	{
		temp = ft_substr(*value, 1, ft_strlen(*value) - 2);
		free(*value);
		*value = temp;
	}
}

void *handle_malloc_error(char **key, char **value, char *str)
{
	if (key)
		free(key);
	if (value)
		free(value);
	perror(str);
	return (NULL);
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
		init_value(&key, &value, arg, eq);
	if (validate_key(key))
	{
		printf("conchinha: export: `%s\': not a valid identifier\n", arg); // check error code thingy
		return (NULL);
	}
	new_var = malloc(sizeof(t_vars));
	if (!new_var)
		return handle_malloc_error(&key, &value, "malloc failed in parse_var");
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
		if (new_var)
			push_list(&head, new_var);
		i++;
	}
	return head;
}

void	add_or_update_var(t_vars **head, char *key, char *value)
{
	t_vars *current;
	t_vars *new_var;

	current = *head;
	while(current)
	{
		if (is_equal(current->key, key))
		{
			free(current->value); // maybe I need to init ft_strdup in a variable so I can free // it's not working so I'll leave it for later
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			return;
		}
		current = current->next;
	}
	new_var = malloc(sizeof(t_vars));
	new_var->key = ft_strdup(key);
	if (value)
		new_var->value = ft_strdup(value);
	else
		new_var->value = NULL;// TODO: Check ft_strdup results
	new_var->next = NULL;
	push_list(head, new_var);
}

static void	swap_vars(t_vars *a, t_vars *b)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = a->key;
	temp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = temp_key;
	b->value = temp_value;
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] > s2[i])
			return (1);
		else if (s1[i] < s2[i])
			return (-1);
		i++;
	}
	return (0);
}

void	sort_vars_list(t_vars *head)
{
	int		swapped;
	t_vars	*current;
	t_vars	*last_ptr;

	last_ptr = NULL;
	if (head == NULL || head->next == NULL)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = head;
		while (current->next != last_ptr)
		{
			if (ft_strcmp(current->key, current->next->key) > 0)
			{
				swap_vars(current, current->next);
				swapped = 1;
			}
			current = current->next;
		}
		last_ptr = current;
	}
}

void	export(t_msh *msh)
{
	int i;
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
