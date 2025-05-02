#include "includes/minishell.h"

void	handle_heredoc(t_command *command)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe error");
		exit(EXIT_FAILURE);
	}
	command->heredoc_fd = pipe_fd[0];
	while (1)
	{
		line = readline("> ");
		if (!line || is_equal(line, command->heredoc_delimiter))
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
}

void	process_heredocs(t_msh *msh)
{
	t_command	*command;

	command = msh->command;
	while (command)
	{
		if (command->is_heredoc)
			handle_heredoc(command);
		command = command->pipe_next;
	}
}
