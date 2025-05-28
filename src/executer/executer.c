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
		msh->exit_status = -1;
		ft_perror(command->name, NULL, msh->exit_status,
			"failed to get status for the last command");
		clear_command_chain(first_command);
	}
	(void)waited_pid;
}

int	single_parent_process(t_msh *msh)
{
	int	saved_stdout_fd;
	int	saved_stdin_fd;
	int	status;

	saved_stdout_fd = dup(STDOUT_FILENO);
	if (saved_stdout_fd < 0)
	{
		perror("minishell: dup failed for saved_stdout_fd");
		return (EXIT_FAILURE);
	}
	saved_stdin_fd = dup(STDIN_FILENO);
	if (saved_stdin_fd < 0)
	{
		perror("minishell: dup failed for saved_stdin_fd");
		close(saved_stdout_fd);
		return (EXIT_FAILURE);
	}
	if (msh->command->input_redirect)
		input_redirection(msh->command);
	if (msh->command->outfile)
		output_redirection(msh->command->outfile);
	status = builtin_router(msh, msh->command);
	if (dup2(saved_stdin_fd, STDIN_FILENO) < 0)
	{
		perror("minishell: dup2 failed to restore stdin");
		dup2(saved_stdout_fd, STDOUT_FILENO);
		status = EXIT_FAILURE;
	}
	close(saved_stdin_fd);
	if (dup2(saved_stdout_fd, STDOUT_FILENO) < 0)
	{
		perror("minishell: dup2 failed to restore stdout");
		status = EXIT_FAILURE;
	}
	close(saved_stdout_fd);
	return (status);
}

void	parent_process(t_msh *msh, t_command *command,
	int *fd, int *prev_pipe_read_fd)
{
	if (*prev_pipe_read_fd != STDIN_FILENO)
		close(*prev_pipe_read_fd);
	if (command->index < msh->num_cmds - 1)
	{
		close(fd[1]);
		*prev_pipe_read_fd = fd[0];
	}
}

static int	setup_command_pipe(t_command *cmd, int num_cmds, int *pipe_fds, t_msh *msh)
{
	if (cmd->index < num_cmds - 1 && num_cmds > 1)
	{
		if (pipe(pipe_fds) == -1)
		{
			perror("minishell: pipe failed");
			msh->exit_status = EXIT_FAILURE;
			return (0);
		}
	}
	return (1);
}

static pid_t	execute_piped_command(t_msh *msh, t_command *cmd, int *pipe_fds, int *prev_pipe_fd_ptr)
{
	pid_t	pid;

	pid = fork();
	cmd->pid = pid;
	if (pid == -1)
	{
		perror("minishell: fork failed");
		msh->exit_status = EXIT_FAILURE;
		if (cmd->index < msh->num_cmds - 1 && msh->num_cmds > 1)
		{
			close(pipe_fds[0]);
			close(pipe_fds[1]);
		}
		return (-1);
	}
	if (pid == 0)
		child_process(msh, cmd, *prev_pipe_fd_ptr, pipe_fds);
	if (*prev_pipe_fd_ptr != STDIN_FILENO)
		close(*prev_pipe_fd_ptr);
	if (cmd->index < msh->num_cmds - 1 && msh->num_cmds > 1)
	{
		close(pipe_fds[1]);
		*prev_pipe_fd_ptr = pipe_fds[0];
	}
	return (pid);
}

static void	execute_pipeline_loop(t_msh *msh, int *prev_pipe_fd_ptr)
{
	t_command	*current_cmd;
	int			pipe_fds[2];
	pid_t		child_pid;

	current_cmd = msh->command;
	while (current_cmd)
	{
		if (!setup_command_pipe(current_cmd, msh->num_cmds, pipe_fds, msh))
			return ;
		child_pid = execute_piped_command(msh, current_cmd, pipe_fds, prev_pipe_fd_ptr);
		if (child_pid == -1)
			return ;
		current_cmd = current_cmd->pipe_next;
	}
}

int	process(t_msh *msh)
{
	int	prev_pipe_read_fd;

	if (!msh->command)
		return (0);
	if (!process_heredocs(msh))
		return (0);
	if (msh->num_cmds == 1 && is_builtin(msh->command->name))
		return (single_parent_process(msh));
	prev_pipe_read_fd = STDIN_FILENO;
	execute_pipeline_loop(msh, &prev_pipe_read_fd);
	if (prev_pipe_read_fd != STDIN_FILENO)
		close(prev_pipe_read_fd);
	wait_for_children(msh, msh->command);
	return (0);
}
// int	process(t_msh *msh)
// {
// 	int			fd[2];
// 	int			status;
// 	int			prev_pipe_read_fd;
// 	pid_t		pid;
// 	t_command	*command;

// 	if (!msh->command)
// 		return (0);
// 	if (!process_heredocs(msh))
// 		return (0);
// 	if (msh->num_cmds == 1 && is_builtin(msh->command->name))
// 		return (single_parent_process(msh));
// 	status = 0;
// 	command = msh->command;
// 	prev_pipe_read_fd = STDIN_FILENO;
// 	while (command)
// 	{
// 		if (command->index < msh->num_cmds - 1 && msh->num_cmds > 1)
// 		{
// 			if (pipe(fd) == -1)
// 			{
// 				perror("minishell: pipe failed");
// 				msh->exit_status = EXIT_FAILURE;
// 				if (prev_pipe_read_fd != STDIN_FILENO)
// 					close(prev_pipe_read_fd);
// 				break ;
// 			}
// 		}
// 		pid = fork();
// 		command->pid = pid;
// 		if (pid == -1)
// 		{
// 			perror("minishell: fork failed");
// 			msh->exit_status = EXIT_FAILURE;
// 			if (command->index < msh->num_cmds - 1 && msh->num_cmds > 1)
// 			{
// 				close(fd[0]);
// 				close(fd[1]);
// 			}
// 			if (prev_pipe_read_fd != STDIN_FILENO)
// 				close(prev_pipe_read_fd);
// 			break ;
// 		}
// 		else if (pid == 0)
// 			child_process(msh, command, prev_pipe_read_fd, fd);
// 		else
// 			parent_process(msh, command, fd, &prev_pipe_read_fd);
// 		command = command->pipe_next;
// 	}
// 	if (prev_pipe_read_fd != STDIN_FILENO)
// 		close(prev_pipe_read_fd);
// 	wait_for_children(msh, msh->command);
// 	return (status);
// }
