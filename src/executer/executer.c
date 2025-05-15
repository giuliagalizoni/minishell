#include "../includes/minishell.h"

void	wait_for_children(t_msh *msh, t_command *first_command)
{
	t_command	*command;
	int			status;
	pid_t		waited_pid;

	status = 0;
	command = first_command;
	while (command)
	{
		waited_pid = waitpid(command->pid, &status, 0);
		command = command->pipe_next;
	}
	if (WIFEXITED(status))
		msh->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		msh->exit_status = 128 + WTERMSIG(status);
	else
	{
		// TODO teardown
		perror("Error: failed to get status for the last command");
		msh->exit_status = -1;
	}
	(void)waited_pid;
}

int	single_parent_process(t_msh *msh)
{
	int	saved_stdout_fd;
	int	saved_stdin_fd;
	int	status;

	// TODO need some error checking somewhere?
	saved_stdout_fd = dup(STDOUT_FILENO);
	saved_stdin_fd = dup(STDIN_FILENO);
	if (msh->command->input_redirect)
		input_redirection(msh->command, msh);
	if (msh->command->outfile)
		output_redirection(msh->command->outfile);
	status = builtin_router(msh);
	if (dup2(saved_stdin_fd, STDIN_FILENO) < 0 || dup2(saved_stdout_fd, STDOUT_FILENO) < 0)
		cleanup_on_error(msh, "dup2 failed", 0);
	return (status);
}

void	child_process(t_msh *msh, t_command *command, int prev_pipe_read_fd, int *fd)
{
	char **envp;

	if (prev_pipe_read_fd != STDIN_FILENO)
	{
		if (dup2(prev_pipe_read_fd, STDIN_FILENO) == -1)
		{
			perror("dup 2 fail for stdin redirection");
			clear_command_chain(msh->command);
			clean_myenv(msh->myenv);
			exit(EXIT_FAILURE);
		}
//			cleanup_on_error(msh, "dup2 fail for stdin redirection", 1);
		close(prev_pipe_read_fd);
	}
	if (command->index < msh->num_cmds - 1)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup 2 fail for stdout redirection");
			clear_command_chain(msh->command);
			clean_myenv(msh->myenv);
			exit(EXIT_FAILURE);
		}
		//	cleanup_on_error(msh, "dup2 fail for stdout redirection", 1);
		close(fd[1]);
	}
	// ***	HEREDOC ***
	if (command->is_heredoc)
	{
		if(dup2(command->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("dup 2 fail for heredoc redirection");
			rl_clear_history();
			clear_command_chain(msh->command);
			clean_myenv(msh->myenv);
			exit(EXIT_FAILURE);
		}
//			cleanup_on_error(msh, "dup2 fail for stdin heredoc redirection", 1);
		close(command->heredoc_fd);
	}
	if (command->input_redirect)
		if (!input_redirection(command, msh))
		{
			rl_clear_history();
			clear_command_chain(msh->command);
			clean_myenv(msh->myenv);
			exit(EXIT_FAILURE);
		}
	if (command->outfile)
		if (!output_redirection(command->outfile))
		{
			rl_clear_history();
			clear_command_chain(msh->command);
			clean_myenv(msh->myenv);
			exit(EXIT_FAILURE);
		}

	if (is_builtin(command->name))
		child_builtin(msh);
	if (!command->path)	
	{

		//TODO make it more robust so it can check folders and
		//permissions?
		rl_clear_history();
		clear_command_chain(msh->command);
		clean_myenv(msh->myenv);
		exit(EXIT_FAILURE);
	}
	else
	{
		envp = myenv_to_envp(msh->myenv);
		if (execve(command->path, command->arguments, envp) == -1)
		{
			//TODO stuck here
			perror("command not found");
			rl_clear_history();
			clear_command_chain(msh->command);
			clean_myenv(msh->myenv);
			free_arr((void **)envp);
			exit(127);
		}
		clear_command_chain(msh->command);
		clean_myenv(msh->myenv);
		free_arr((void **)envp);

		exit(EXIT_SUCCESS);
	}
}

void	parent_process(t_msh *msh, t_command *command, int *fd, int *prev_pipe_read_fd)
{
	if (*prev_pipe_read_fd != STDIN_FILENO)
		close(*prev_pipe_read_fd);
	if (command->index < msh->num_cmds - 1)
	{
		close(fd[1]);
		*prev_pipe_read_fd = fd[0];
	}
}

int	process(t_msh *msh)
{
	int			fd[2];
	int			status;
	int			prev_pipe_read_fd;
	pid_t		pid;
	t_command	*command;
	/* TODO ERROR HANDLING
	 *
	 * process_heredoc fds
	 */

	if (!process_heredocs(msh))
		return (0);
	if (msh->num_cmds == 1 && is_builtin(msh->command->name))
		return (single_parent_process(msh));
	status = 0;
	command = msh->command;
	prev_pipe_read_fd = STDIN_FILENO;
	if (pipe(fd) == -1)
		perror("pipe fail"); // TODO error handling
	while (command)
	{
		if (command->index < msh->num_cmds - 1 && msh->num_cmds > 1)
		{
			if (pipe(fd) == -1)
			{
				//TODO cleanup cmd chain, print error, set exit
				// return 0 or 1 or whatever
				perror("pipe fail");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		command->pid = pid;
		if (pid == -1)
		{
			//TODO cleanup cmd chain, print error, set exit
			// return 0 or 1 or whatev
			perror("fork fail");
		}
		else if (pid == 0)
			child_process(msh, command, prev_pipe_read_fd, fd);
		else
			parent_process(msh, command, fd, &prev_pipe_read_fd);
		command = command->pipe_next;
	}
	wait_for_children(msh, msh->command);
	close(fd[0]);
	close(fd[1]);
	return (status);
}
