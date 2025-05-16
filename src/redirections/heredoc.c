#include "../includes/minishell.h"

static void	write_line(int	*pipe_fd, char *expanded_line)
{
	write(pipe_fd[1], expanded_line, ft_strlen(expanded_line));
	write(pipe_fd[1], "\n", 1);
	free(expanded_line);
}

void	handle_heredoc(t_command *command, t_msh *msh)
{
	int		pipe_fd[2];
	char	*line;
	char	*expanded_line;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe error");
		exit(EXIT_FAILURE); // TODO handle exit status
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
		expanded_line = expand_inline(line, msh);
		free(line);
		if (expanded_line)
			write_line(pipe_fd, expanded_line);
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
			handle_heredoc(command, msh);
		command = command->pipe_next;
	}
}
