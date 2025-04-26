#include "includes/minishell.h"
// TODO
// split the function into process / child, see what needs to be passed around
// put the pids in the command struct
// flesh out error handling a bit more (free the command object at every
// error?)
// see if we can get rid of the prev_pipe_read_fd, i don't like it


int	wait_for_children(t_command *first_command)
{
	t_command *command;
	int	status;
	// we don't need this final_status no more cos it's global now
	int	final_status;
	pid_t	waited_pid;

	// TODO review this a bit according to this
	// https://gemini.google.com/app/a5ea8cc40aba5f5e
	// but maybe it's fine like this
	command = first_command;
	while (command)
	{
		waited_pid = waitpid(command->pid, &status, 0);
		if (waited_pid == -1)
			perror("waitpid failed"); // TODO cleanup?
		command = command->pipe_next;
	}
	if (WIFEXITED(status))
	{
		g_exit_code = WEXITSTATUS(status);
		final_status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		final_status = 128 + WTERMSIG(status);
		g_exit_code = 128 + WTERMSIG(status);
	}
	else
		final_status = -1;
	return (final_status);
}

static void	input_redirection(t_command *command)
{
	int	i;
	int	file;

	i = 0;
	while(command->input_redirect[i])
	{
		file = open(command->input_redirect[i], O_RDONLY);
		if (file == -1)
			perror("Bad file descriptor");// cleanup routine here
		dup2(file, 0);
		close(file);
		i++;
	}
}

static void	output_redirection(t_outfile *outfile)
{
	int	file;

	while(outfile)
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

int	single_parent_process(t_msh *msh)
{
	int	saved_stdout_fd;
	int	saved_stdin_fd;
	int	status;

	saved_stdout_fd = dup(STDOUT_FILENO);
	saved_stdin_fd = dup(STDIN_FILENO);
	if (msh->command->input_redirect)
		input_redirection(msh->command);
	if (msh->command->outfile)
		output_redirection(msh->command->outfile);
	status = builtin_router(msh);
	dup2(saved_stdin_fd , STDIN_FILENO);
	dup2(saved_stdout_fd , STDOUT_FILENO);
	return (status);
}

void	sigquit_handler(int signal)
{
	//TODO need to 
	if (signal == SIGQUIT)
	{
		g_exit_code = 131;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_quit_action(void)
{
	struct sigaction sa_quit;

	ft_bzero(&sa_quit, sizeof(sa_quit));
	sa_quit.sa_handler = &sigquit_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	// TODO wrap up in a in if sigaction == -1 for error check
	sigaction(SIGQUIT, &sa_quit, NULL);
}

struct sigaction	sigignore(int signal)
{
	struct sigaction	sa;
	struct sigaction	sa_old;

	ft_bzero(&sa, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(signal, &sa, &sa_old);

	return (sa_old);
}

void	child_process(t_msh *msh, int prev_pipe_read_fd, int *fd)
{

	// TODO do i really need this prev_pipe_read_fd
	// if it's not the first cmd, redirect input
	set_quit_action();
	if (prev_pipe_read_fd != STDIN_FILENO)
	{
		if (dup2(prev_pipe_read_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 fail for stdin redirection");
			exit(EXIT_FAILURE);
		}
		close(prev_pipe_read_fd);
	}
	// if it's not the last cmd, redirect output
	if (msh->command->index < msh->num_cmds - 1)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed for stdout redirection");
			exit(EXIT_FAILURE);
		}
		close(fd[1]);
	}
	//TODO what to do with builtins?if i just move his code to process it
	//hangs. Mayb just copy it to the builtin router?
	if (msh->command->input_redirect)
		input_redirection(msh->command);
	if (msh->command->outfile)
		output_redirection(msh->command->outfile);
	if (is_builtin(msh->command->name))
		child_builtin(msh);
	else
	{
		execve(msh->command->path, msh->command->arguments, NULL);
		// better command not found error here
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
}

void	parent_process(t_msh *msh, int *fd, int *prev_pipe_read_fd)
{

	if (*prev_pipe_read_fd != STDIN_FILENO)
		close(*prev_pipe_read_fd);
	if (msh->command->index < msh->num_cmds - 1)
	{
		close(fd[1]);
		*prev_pipe_read_fd = fd[0];
	}
}

int	process(t_msh *msh)
{
	int	fd[2];
	int	status;
	int	prev_pipe_read_fd;
	t_command	*first_command;
	struct sigaction	sa_int_old;
	struct sigaction	sa_quit_old;

	if (msh->num_cmds == 1 && is_builtin(msh->command->name))
		return (single_parent_process(msh));
	status = 0;
	first_command = msh->command;
	prev_pipe_read_fd = STDIN_FILENO;
	if (pipe(fd) == -1)
		perror("pipe fail");
	while (msh->command)
	{
		// if not last cmd, if
		if (msh->command->index < msh->num_cmds - 1 && msh->num_cmds > 1)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe fail");
				exit(EXIT_FAILURE);
			}
		}
		pid_t pid = fork();
		msh->command->pid = pid;
		if (pid == -1)
		{
			perror("fork fail");
			//some cleanup, close fds, free pids
		}
		else if (pid == 0)
			child_process(msh, prev_pipe_read_fd, fd);
		else
		{
			sa_int_old = sigignore(SIGINT);
			sa_quit_old = sigignore(SIGQUIT);
			parent_process(msh, fd, &prev_pipe_read_fd);
		}
		msh->command = msh->command->pipe_next;
	}
	status = wait_for_children(first_command);
	//printf("status: %d\n", status);
	sigaction(SIGINT, &sa_int_old, NULL);
	sigaction(SIGQUIT, &sa_quit_old, NULL);
	close(fd[0]);
	close(fd[1]);
	return (status);
}
