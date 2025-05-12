#include "../includes/minishell.h"

void	command_init(t_command *command)
{
	command->name = NULL;
	command->arguments = NULL;
	command->input_redirect = NULL;
	command->is_heredoc = 0;
	command->heredoc_delimiter = NULL;
	command->heredoc_fd = -1;
	command->outfile = NULL;
	command->is_pipe = 0;
	command->pipe_next = NULL;
	command->index = 0;
}

// void	set_command_paths(t_command *command, char **envp)
// {
// 	char	*full_cmd_path;

// 	while (command)
// 	{
// 		full_cmd_path = get_cmd_path(command->name, envp);
// 		free(command->name);
// 		command->name = full_cmd_path;
// 		command = command->pipe_next;
// 	}
// }

void	clear_command_chain(t_command *command)
{
	if (!command)
		return ;
	if (command->pipe_next)
	{
		clear_command_chain(command->pipe_next);
		command->pipe_next = NULL;
	}
	if (command->name)
		free(command->name);
	if (command->path)
		free(command->path);
	if (command->arguments)
		free_arr((void **)command->arguments);
	if (command->input_redirect)
		free_arr((void **)command->input_redirect);
	if (command->heredoc_delimiter)
		free(command->heredoc_delimiter);
	if (command->outfile)
	{
		if (command->outfile->filename)
			free(command->outfile->filename);
		free(command->outfile);
		command->outfile = NULL;
	}
	free(command);
}

// void	clear_command_chain(t_command *command)
// {
// 	t_command	*tmp;

// 	if (!command)
// 		return ;
// 	while (command)
// 	{
// 		tmp = command->pipe_next;
// 		if (command->name)
// 			free(command->name);
// 		if (command->path)
// 			free(command->path);
// 		if (command->arguments)
// 			free_arr((void **)command->arguments);
// 		if (command->input_redirect)
// 			free(command->input_redirect);
// 		if (command->heredoc_delimiter)
// 			free(command->heredoc_delimiter);
// 		/* TODO change this to the outfile thing
// 		if (command->output_redirect)
// 			free(command->output_redirect);
// 			*/
// 		if (command->outfile)
// 		{
// 			free(command->outfile->filename);
// 			free(command->outfile);
// 			command->outfile = NULL;
// 		}
// 		free(command);
// 		command = tmp;
// 	}
// 	free(tmp);
// 	free(command);
// }

int	count_commands(t_command *command)
{
	int	count;

	count = 0;
	while (command)
	{
		count++;
		command = command->pipe_next;
	}
	return (count);
}
