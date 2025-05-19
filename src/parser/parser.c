#include "../includes/minishell.h"

static void	init_pipe(t_command *command, char **tokens, int *i, t_msh *msh)
{
	command->is_pipe = 1;
	tokens++;
	// TODO check if this command is NULL
	command->pipe_next = analyser(tokens + ((*i)++), (command->index) + 1, msh);
}

// Returns 1 on success, 0 on failure
static int	check_operators(t_command *command, char **tokens, int i)
{
	if (is_equal(tokens[i], "<<"))
	{
		command->is_heredoc = 1;
		command->heredoc_is_final = 1;
	}
	else if (is_equal(tokens[i], "<"))
		command->heredoc_is_final = 0;
	else if (i > 0 && is_equal(tokens[i - 1], "<"))
	{
		if (!arr_push(&command->input_redirect, tokens[i]))
			return (0); // arr_push failed, propagate error
	}
	else if (i > 0 && is_equal(tokens[i - 1], "<<"))
	{
		if (command->heredoc_delimiter)
			free(command->heredoc_delimiter);
		command->heredoc_delimiter = ft_strdup(tokens[i]);
		if (!command->heredoc_delimiter)
		{
			perror("ft_strdup failed for heredoc_delimiter");
			return (0); // ft_strdup failed, propagate error
		}
	}
	else
	{
		if (!arr_push(&command->arguments, tokens[i]))
			return (0); // arr_push failed, propagate error
	}
	return (1); // Success
}

// Returns 1 on success, 0 on failure (e.g., malloc error)
static int	set_name(t_command *command, char **tokens, t_vars *myenv)
{
	int	i;

	i = 0;
	// It's assumed tokens and tokens[i] are valid based on prior lexing/parsing stages.
	// If tokens[i] could be NULL here, further checks would be needed.
	while (tokens[i] && tokens[i][0] == '<') // Added check for tokens[i]
		i = i + 2;
	if (!tokens[i]) // If no command name found after skipping redirections
	{
		p_syntax_error(NULL); // Or a more specific error
		return (0);
	}
	command->name = ft_strdup(tokens[i]);
	if (!command->name)
	{
		perror("malloc failed for command name");
		return (0); // Indicate failure
	}
	command->path = get_cmd_path(command->name, myenv); // Use command->name here
	// No need to free command->name if get_cmd_path fails, path can be NULL.
	return (1); // Indicate success
}

t_command	*analyser(char **tokens, int index, t_msh *msh)
{
	t_command	*command;
	int			i;

	if (!tokens || !tokens[0]) // If tokens is NULL or empty, it's an error or no command
		return (NULL);
	command = malloc(sizeof(t_command));
	if (!command)
	{
		perror("malloc failed for command struct");
		return (NULL);
	}
	command_init(command);
	command->index = index;
	// Call set_name and check its return value.
	if (!set_name(command, tokens, msh->myenv))
	{
		free(command); // Free the allocated command struct
		return (NULL); // Propagate the error
	}
	i = 0;
	while (tokens[i])
	{
		if (is_equal(tokens[i], "|"))
		{
			init_pipe(command, tokens, &i, msh);
			// If init_pipe can fail due to malloc, its error should be propagated.
			// For now, assume init_pipe handles its errors or they are caught by command->pipe_next being NULL.
			break ;
		}
		else if (is_equal(tokens[i], ">") || is_equal(tokens[i], ">>"))
		{
			// add_outfile now returns a status. Check it.
			if (!add_outfile(command, tokens, &command->outfile, &i))
			{
				clear_command_chain(command); // Free partially built command
				return (NULL); // Propagate error
			}
		}
		else
		{
			if (!check_operators(command, tokens, i))
			{
				clear_command_chain(command); // Free partially built command
				return (NULL); // Propagate error
			}
		}
		i++;
	}
	return (command);
}

t_command	*parser(char *line, t_msh *msh)
{
	char	**tokens;
	char	**retokens;

	if (!line)
		return (NULL);
	tokens = NULL;
	if (!lexer(line, &tokens))
		return (NULL);
	if (!tokens)
		return (NULL);
	if (tokens && !check_invalid_syntax(tokens))
	{
		free_arr((void **)tokens);
		tokens = NULL;
		return (NULL);
	}
	retokens = expand_and_retokenize(tokens, msh);
	msh->command = analyser(retokens, 0, msh);
	free_arr((void **)tokens);
	if (!msh->command)
	{
		// retokens is now freed unconditionally later, so no need to free here.
		// No specific error message needed here as downstream functions handle it.
	}
	free_arr((void **)retokens); // Unconditionally free retokens here
	return (msh->command);
}
