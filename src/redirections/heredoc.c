#include "../includes/minishell.h"

static void	write_to_heredoc_pipe(int write_fd, char *expanded_line)
{
	write(write_fd, expanded_line, ft_strlen(expanded_line));
	write(write_fd, "\n", 1);
	free(expanded_line);
}

static int	manage_heredoc_line(char *line, t_command *command,
						t_msh *msh, int write_fd)
{
	char	*expanded_line;

	/*
	if (g_signal_code == SIGINT)
	{
		msh->exit_status = 130;
		if (line)
			free(line);
		return (-1);
	}
	*/
	if (!line)
	{
		ft_perror(command->name, NULL, 1,
			"warning: here-document delimited by end-of-file");
		return (0);
	}
	if (is_equal(line, command->heredoc_delimiter))
	{
		free(line);
		return (0);
	}
	expanded_line = expand_inline(line, msh);
	free(line);
	if (expanded_line)
		write_to_heredoc_pipe(write_fd, expanded_line);
	return (1);
}

int	readline_interrupt_event_hook()
{
	if (g_signal_code == 2)
		rl_done = 1;
	return (0);
}

int	handle_heredoc(t_command *command, t_msh *msh)
{
	char	*current_line;
	int		line_status;
	int		status;
//	int		pid;

	status = 0;
	if (pipe(command->heredoc_fd) == -1)
		error_cleanup(msh);
//	pid = fork();

	/*
	if (pid == -1)
		perror("minishell: fork failed");
	if (pid == 0)
	{
	*/
	set_signals_heredoc();
	rl_event_hook = readline_interrupt_event_hook;
	while (1)
	{
		if (g_signal_code == 2)
			break ;
		current_line = readline("> ");
		if (g_signal_code == 2)
		{
			close(command->heredoc_fd[1]);
			g_signal_code = -1;
			set_signals_parent();
			return (0);
		}
		line_status = manage_heredoc_line(current_line, command,
				msh, command->heredoc_fd[1]);
		/*
		if (line_status == -1)
			msh->exit_status = 130;
		*/
		if (line_status == 0)
			break ;
	}
		/*
		close(command->heredoc_fd[0]);
		close(command->heredoc_fd[1]);
		exit(0);
	}
	else
	{
		waitpid(pid, &status, 0);
	*/
	close(command->heredoc_fd[1]);
	g_signal_code = -1;
	set_signals_parent();
		/*
		if (WIFEXITED(status))
		{
			msh->exit_status = WEXITSTATUS(status);
			if (msh->exit_status == 130)
				return (0);
		}
		*/
	if (msh->exit_status == 130)
		return (0); //}
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
