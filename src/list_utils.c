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
	//TODO change to is_equal
	if (is_equal(tokens[*i], ">"))
		outfile->is_append = 0;
	else if (is_equal(tokens[*i], ">>"))
		outfile->is_append = 1;
	outfile->filename = ft_strdup(tokens[*i + 1]);
	(*i)++;
	if (!*outfiles)
	{
		printf("adding first outfile %s with redirection=%d\n", outfile->filename, outfile->is_append);
		*outfiles = outfile;
		return ;
	}
	printf("currently at outfile nr %d with name %s and redirection %d\n", *i, outfile->filename, outfile->is_append);
	tmp = filelast(*outfiles);
	printf("adding it to last outfile named %s\n", tmp->filename);
	tmp->next = outfile;
	outfile->next = NULL;
}
