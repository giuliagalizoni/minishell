#include "includes/minishell.h"

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

	(void)cmd;
	outfile = malloc(sizeof(t_outfile));
	if (outfile == NULL)
		perror("Error allocating memory"); // TODO the usual, fix
						   // cleanup
	if (is_equal(tokens[*i], ">"))
		outfile->is_append = 0;
	else if (is_equal(tokens[*i], ">>"))
		outfile->is_append = 1;
	outfile->filename = ft_strdup(tokens[*i + 1]);
	outfile->next = NULL;
	(*i)++;
	if (!*outfiles)
	{
		*outfiles = outfile;
		return ;
	}
	tmp = filelast(*outfiles);
	tmp->next = outfile;
}


/*
 * TODO need to clear up that lsit of files but this is not it
void	filelistclear(t_outfile **outfiles)
{
	if ((*outfiles)->next)
		filelistclear(&(*outfiles)->next);
	if (*outfiles)
	{
		free(&(*outfiles)->filename);
		free(*outfiles);
	}

	outfiles = NULL;
}
*/

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
