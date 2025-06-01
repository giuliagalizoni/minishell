#include "../includes/minishell.h"

static int	file_redirection_error(t_outfile *outfile)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(outfile->filename, 2);
	if (is_directory(outfile->filename))
		ft_putstr_fd(": is a directory\n", 2);
	else
		ft_putstr_fd(": No such file or directory\n", 2);
	return (0);
}

int	input_redirection(t_command *command)
{
	int	i;
	int	file;

	i = 0;
	while (command->input_redirect[i])
	{
		file = open(command->input_redirect[i], O_RDONLY);
		if (file == -1)
			return (0);
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
			return (file_redirection_error(outfile));
		if (dup2(file, 1) == -1)
		{
			ft_putstr_fd("minishell: dup2 failed\n", 2);
			close(file);
			return (0);
		}
		close(file);
		outfile = outfile->next;
	}
	return (1);
}
