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
	(*i)++;
	if (!*outfiles)
	{
		*outfiles = outfile;
		return ;
	}
	tmp = filelast(*outfiles);
	tmp->next = outfile;
	outfile->next = NULL;
}
