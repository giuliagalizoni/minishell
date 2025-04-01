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
}
