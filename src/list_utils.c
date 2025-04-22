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
