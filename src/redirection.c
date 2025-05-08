#include "includes/minishell.h"

void	input_redirection(t_command *command)
{
	int	i;
	int	file;

	i = 0;
	while (command->input_redirect[i])
	{
		file = open(command->input_redirect[i], O_RDONLY);
		if (file == -1)
			perror("Bad file descriptor");// cleanup routine here
		dup2(file, 0);
		close(file);
		i++;
	}
}

void	output_redirection(t_outfile *outfile)
{
	int	file;

	while (outfile)
	{
		if (outfile->is_append)
			file = open(outfile->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			file = open(outfile->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (file == -1)
			perror("Bad file descriptor");// cleanup routine here
		dup2(file, 1);
		close(file);
		outfile = outfile->next;
	}
}
