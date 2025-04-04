#include "includes/minishell.h"

// TODO delete command lists adapted from list delete fns
// move command_init here
void	command_init(t_command *command)
{
	command->name = NULL;
	command->arguments = NULL;
	command->input_redirect = NULL;
	command->is_heredoc = 0;
	command->heredoc_delimiter = NULL;
	command->output_redirect = NULL;
	command->append_output = 0;
	command->is_pipe = 0;
	command->next = NULL;
	command->pipe_next = NULL;
	command->index = 0;
}

void	set_command_paths(t_command *command, char **envp)
{
	char	*full_cmd_path;

	// TODO maybe move this to the parser
	while (command)
	{
		full_cmd_path = get_cmd_path(command->name, envp);
		free(command->name);
		command->name = full_cmd_path;
		command = command->pipe_next;
	}
}

void	clear_command_chain(t_command *command)
{
	t_command	*tmp;

	while (command)
	{
		tmp = command->pipe_next;
		free(command->name);
		free_arr((void **)command->arguments);
		/*
		free(command->input_redirect);
		free(command->heredoc_delimiter);
		free(command->output_redirect);
		free(command->next);
		*/
		//free(command);
		command = tmp;
	}
}

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
