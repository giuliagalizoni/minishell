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

void	add_outfile(t_command *cmd, char **tokens, t_outfile **outfiles, int *i)
{
	t_outfile	*outfile;
	t_outfile	*tmp;
	char		*filename;

	(void)cmd;
	outfile = malloc(sizeof(t_outfile));
	if (outfile == NULL)
	{
		perror("Error allocating memory for outfile");
		return;
	}
	if (is_equal(tokens[*i], ">"))
		outfile->is_append = 0;
	else if (is_equal(tokens[*i], ">>"))
		outfile->is_append = 1;
	filename = ft_strdup(tokens[*i + 1]);
	if (!filename)
	{
		free(outfile);
		perror("Error duplicating filename");
		return;
	}
	outfile->filename = filename;
	outfile->next = NULL;
	(*i)++;
	if (!*outfiles)
	{
		*outfiles = outfile;
		return;
	}
	tmp = filelast(*outfiles);
	if (!tmp)
	{
		free(outfile->filename);
		free(outfile);
		perror("Error finding last outfile");
		return;
	}
	tmp->next = outfile;
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
		return;
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
