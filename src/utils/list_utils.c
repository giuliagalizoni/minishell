#include "../includes/minishell.h"

t_outfile	*filelast(t_outfile *outfile)
{
	t_outfile	*tmp;

	tmp = outfile;
	if (tmp == NULL)
		return (NULL);
	while (tmp->next != NULL)
		tmp = tmp->next;
	return (tmp);
}

// Returns 1 on success, 0 on failure
int	add_outfile(t_command *cmd, char **tokens, t_outfile **outfiles, int *i)
{
	t_outfile	*outfile;
	t_outfile	*tmp;

	(void)cmd;
	outfile = malloc(sizeof(t_outfile));
	if (outfile == NULL)
	{
		perror("malloc failed for outfile struct in add_outfile");
		return (0); // Allocation failed
	}
	if (is_equal(tokens[*i], ">"))
		outfile->is_append = 0;
	else if (is_equal(tokens[*i], ">>"))
		outfile->is_append = 1;

	// tokens[*i + 1] should be the filename. Ensure it exists.
	if (!tokens[*i + 1])
	{
		p_syntax_error(NULL); // Or specific error for missing filename
		free(outfile);
		return (0);
	}
	outfile->filename = ft_strdup(tokens[*i + 1]);
	if (!outfile->filename)
	{
		perror("ft_strdup failed for outfile filename in add_outfile");
		free(outfile); // Free the allocated outfile struct
		return (0); // Allocation failed
	}
	outfile->next = NULL;
	(*i)++; // Increment i *after* using tokens[i] and tokens[i+1]
	if (!*outfiles)
	{
		*outfiles = outfile;
		return (1);
	}
	tmp = filelast(*outfiles);
	tmp->next = outfile;
	return (1); // Success
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
