#include "../includes/minishell.h"

static void	handle_child_pipe_io(t_msh *msh, t_command *cmd,
	int prev_p_fd, int *p_fds)
{
	if (prev_p_fd != STDIN_FILENO)
	{
		if (dup2(prev_p_fd, STDIN_FILENO) == -1)
			exit_process(msh, cmd, NULL, "dup2 for stdin failed");
		close(prev_p_fd);
	}
	if (cmd->index < msh->num_cmds - 1)
	{
		close(p_fds[0]);
		if (dup2(p_fds[1], STDOUT_FILENO) == -1)
			exit_process(msh, cmd, NULL, "dup2 for stdout failed");
		close(p_fds[1]);
	}
}

static void	handle_child_file_redirections(t_msh *msh, t_command *cmd)
{
	char	*input_target;

	input_target = NULL;
	if (cmd->heredoc_is_final)
	{
		if (cmd->is_heredoc && cmd->heredoc_fd != -1)
		{
			if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
				exit_process(msh, cmd, NULL, "dup2 for heredoc failed");
			close(cmd->heredoc_fd);
		}
	}
	else if (cmd->input_redirect && cmd->input_redirect[0] != NULL)
	{
		if (cmd->input_redirect[0])
			input_target = cmd->input_redirect[0];
		if (!input_redirection(cmd))
			exit_process(msh, cmd, input_target, NULL);
	}
	if (cmd->outfile)
	{
		if (!output_redirection(cmd->outfile))
			exit_process(msh, cmd, NULL, "output redirection failed");
	}
}

static void	validate_command_for_execution(t_msh *msh, t_command *cmd)
{
	if (!cmd->name)
	{
		msh->exit_status = 0;
		exit_process(msh, cmd, NULL, NULL);
	}
	if (!cmd->path)
	{
		msh->exit_status = 127;
		exit_process(msh, cmd, NULL, "command not found");
	}
	if (is_directory(cmd->path))
	{
		msh->exit_status = 126;
		exit_process(msh, cmd, NULL, "Is a directory");
	}
	if (access(cmd->path, X_OK) == -1)
	{
		msh->exit_status = 126;
		exit_process(msh, cmd, NULL, "Permission denied");
	}
}

static void	execute_command_in_child(t_msh *msh, t_command *cmd)
{
	char	**envp;

	if (is_builtin(cmd->name))
		child_builtin(msh, cmd);
	validate_command_for_execution(msh, cmd);
	envp = myenv_to_envp(msh->myenv);
	if (!envp && msh->myenv)
		exit_process(msh, cmd, NULL, "failed to create environment");
	execve(cmd->path, cmd->arguments, envp);
	free_arr((void **)envp);
	msh->exit_status = 127;
	exit_process(msh, cmd, cmd->name, "execve failed");
}

void	child_process(t_msh *msh, t_command *command,
	int prev_pipe_read_fd, int *fd)
{
	msh->exit_status = 1;
	handle_child_pipe_io(msh, command, prev_pipe_read_fd, fd);
	handle_child_file_redirections(msh, command);
	execute_command_in_child(msh, command);
}
