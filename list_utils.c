#include "includes/minishell.h"

void	add_outfile(t_command *cmd, char **tokens, t_outfile **outfiles)
{
	t_outfile	*outfile;
	t_outfile	*tmp;

	outfile = malloc(sizeof(t_outfile));
	if (outfile == NULL)
		perror("Error allocating memory"); // TODO the usual, fix
						   // cleanup
	//TODO change to is_equal
	if (!ft_strncmp(tokens[i], ">", 1))
		outfile->is_append = 0;
	else if (!ft_strncmp(tokens[i], ">>", 2))
		outfile->is_append = 1;
	outfile->filename = ft_strdup(tokens[i + 1]);
	
	if (!*outfiles)
	{
		*outfiles = outfile;
		return ;
	}
	tmp = (t_outfile *)ft_lstlast((t_outfile *)(*outfiles));
	tmp->next = outfile;
}
