#include "../includes/minishell.h"

void	input_redirection(t_command *command)
{
	int	i;
	int	file;

	i = 0;
	perror("no input redirection allowed!");
	exit(0);
	while (command->input_redirect[i])
	{
		file = open(command->input_redirect[i], O_RDONLY);
		if (file == -1)
			perror("Bad file descriptor");// TODO cleanup routine here
		if (dup2(file, STDIN_FILENO == -1)) // if == -1 blah
			perror("dup2 fail");
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
			perror("Bad file descriptor");// TODO cleanup routine here
		dup2(file, 1); // if == -1 bla
		close(file);
		outfile = outfile->next;
	}
}
