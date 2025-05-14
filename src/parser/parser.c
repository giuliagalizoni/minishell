#include "../includes/minishell.h"

static void	init_pipe(t_command *command, char **tokens, int *i, t_msh *msh)
{
	command->is_pipe = 1;
	tokens++;
	// TODO check if this command is NULL
	command->pipe_next = analyser(tokens + ((*i)++), (command->index) + 1, msh);
}

static void	check_operators(t_command *command, char **tokens, int i)
{
	if (is_equal(tokens[i], "<<"))
		command->is_heredoc = 1;
	else if (is_equal(tokens[i], "<"))
		command->is_heredoc = 0;
	else if (i > 0 && is_equal(tokens[i - 1], "<"))
		arr_push(&command->input_redirect, tokens[i]);
	else if (i > 0 && is_equal(tokens[i - 1], "<<"))
		command->heredoc_delimiter = ft_strdup(tokens[i]); //TODO check
								   //if this
								   //strdup
								   //NULL
	else
		arr_push(&command->arguments, tokens[i]);
}

void	set_name(t_command *command, char **tokens, t_vars *myenv)
{
	int	i;

	i = 0;
	while (tokens[i][0] == '<')
		i = i + 2;
	// TODO handle this malloc
	command->name = ft_strdup(tokens[i]);
	command->path = get_cmd_path(tokens[i], myenv);
}

t_command	*analyser(char **tokens, int index, t_msh *msh)
{
	t_command	*command;
	int			i;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command_init(command);
	command->index = index;
	set_name(command, tokens, msh->myenv);
	i = 0;
	while (tokens[i])
	{
		if (is_equal(tokens[i], "|"))
		{
			init_pipe(command, tokens, &i, msh);
			break ;
		}
		else if (is_equal(tokens[i], ">") || is_equal(tokens[i], ">>"))
			add_outfile(command, tokens, &command->outfile, &i);
		else
			check_operators(command, tokens, i);
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
		free_arr((void **)retokens);
	}
	free_arr((void **)retokens);
	return (msh->command);
}
