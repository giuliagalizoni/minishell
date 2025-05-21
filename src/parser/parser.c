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
			return (0);
	}
	else if (i > 0 && is_equal(tokens[i - 1], "<<"))
	{
		if (command->heredoc_delimiter)
			free(command->heredoc_delimiter);
		command->heredoc_delimiter = ft_strdup(tokens[i]);
		if (!command->heredoc_delimiter)
		{
			perror("ft_strdup failed for heredoc_delimiter");
			return (0);
		}
	}
	else
	{
		if (!arr_push(&command->arguments, tokens[i]))
			return (0);
	}
	return (1);
}

// Returns 1 on success, 0 on failure (e.g., malloc error)
static int	set_name(t_command *command, char **tokens, t_vars *myenv)
{
	int	i;

	i = 0;
	while (tokens[i] && tokens[i][0] == '<')
		i = i + 2;
	if (!tokens[i])
	{
		p_syntax_error(NULL);
		return (0);
	}
	command->name = ft_strdup(tokens[i]);
	if (!command->name)
	{
		perror("malloc failed for command name");
		return (0);
	}
	command->path = get_cmd_path(command->name, myenv);
	return (1);
}

t_command	*analyser(char **tokens, int index, t_msh *msh)
{
	t_command	*command;
	int			i;

	if (!tokens || !tokens[0])
		return (NULL);
	command = malloc(sizeof(t_command));
	if (!command)
		return (perror("malloc failed for command struct"), NULL);
	command_init(command);
	command->index = index;
	if (!set_name(command, tokens, msh->myenv))
		return (free(command), NULL);
	i = 0;
	while (tokens[i])
	{
		if (is_equal(tokens[i], "|"))
		{
			init_pipe(command, tokens, &i, msh);
			break ;
		}
		else if (is_equal(tokens[i], ">") || is_equal(tokens[i], ">>"))
		{
			if (!add_outfile(command, tokens, &command->outfile, &i))
				return (clear_command_chain(command), NULL);
		}
		else
		{
			if (!check_operators(command, tokens, i))
				return (clear_command_chain(command), NULL);
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
	free_arr((void **)retokens);
	return (msh->command);
}
