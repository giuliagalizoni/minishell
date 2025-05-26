#include "../includes/minishell.h"

int	input_redirection(t_command *command)
{
	int	i;
	int	file;

	i = 0;
	while (command->input_redirect[i])
	{
		file = open(command->input_redirect[i], O_RDONLY);
		if (file == -1)
		{
			// TODO use the error function. 
			perror("Bad file descriptor");
			return (0);
	        }
		if (dup2(file, STDIN_FILENO == -1))
		{
			perror("dup2 fail");
			close(file);
			return (0);
		}
		close(file);
		i++;
	}
	return (1);
}

int	output_redirection(t_outfile *outfile)
{
	int	file;

	while (outfile)
	{
		if (outfile->is_append)
			file = open(outfile->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			file = open(outfile->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (file == -1)
		{
			// TODO use the error function. 
			// make sure file is created if we get a cmd like cat -e > test1 < test2
			perror("Bad file descriptor");
			return (0);
		}
		if (dup2(file, 1) == -1)
		{
			perror("dup2 failed");
			close(file);
			return (0);
		}
		close(file);
		outfile = outfile->next;
	}
	return (1);
}
