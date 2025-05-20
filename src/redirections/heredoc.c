#include "../includes/minishell.h"

static void	write_line(int	*pipe_fd, char *expanded_line)
{
	write(pipe_fd[1], expanded_line, ft_strlen(expanded_line));
	write(pipe_fd[1], "\n", 1);
	free(expanded_line);
}

int	handle_heredoc(t_command *command, t_msh *msh)
{
	int		pipe_fd[2];
	char	*line;
	char	*expanded_line;

	if (pipe(pipe_fd) == -1)
		error_cleanup(msh, "pipe error");
	command->heredoc_fd = pipe_fd[0];
	while (1)
	{
		line = readline("> ");
		if (!line || is_equal(line, command->heredoc_delimiter))
		{
			free(line);
			break ;
		}
		expanded_line = expand_inline(line, msh);
		free(line);
		if (expanded_line)
			write_line(pipe_fd, expanded_line);
	}
	close(pipe_fd[1]);
	return (1);
}

int	process_heredocs(t_msh *msh)
{
	t_command	*command;

	command = msh->command;
	while (command)
	{
		if (command->is_heredoc)
			if (!handle_heredoc(command, msh))
				return (0);
		command = command->pipe_next;
	}
	return (1);
}
