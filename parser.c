#include "includes/minishell.h"

t_command	analyser(char **tokens, int index);

static void	init_pipe(t_command *command, char **tokens, int *i, int *index)
{
	command->is_pipe = 1;
	tokens++;
	command->pipe_next = malloc(sizeof(t_command));
	if (!command->pipe_next)
		return ;
	*command->pipe_next = analyser(tokens + ((*i)++), (*index) + 1);
}

static void	check_operators(t_command *command, char **tokens, int i)
{
	if (!ft_strncmp(tokens[i], "<<", 2))
		command->is_heredoc = 1;
	else if (!ft_strncmp(tokens[i], "<", 1))
		command->is_heredoc = 0;
	else if (i > 0 && !ft_strncmp(tokens[i-1], "<", 1))
		command->input_redirect = ft_strdup(tokens[i]);
	else if (i > 0 && !ft_strncmp(tokens[i-1], "<<", 2))
		command->heredoc_delimiter = ft_strdup(tokens[i]);
	else if (!ft_strncmp(tokens[i], ">>", 2))
		command->append_output = 1;
	else if (!ft_strncmp(tokens[i], ">", 1))
		command->append_output = 0;
	else if (i > 0 && !ft_strncmp((tokens[i-1]), ">>", 2))
		command->output_redirect = ft_strdup(tokens[i]);
	else if (i > 0 && !ft_strncmp(tokens[i-1], ">", 1))
		command->output_redirect = ft_strdup(tokens[i]);
	else
		arr_push(&command->arguments, tokens[i]);
}
t_command	analyser(char **tokens, int index)
{
	t_command command;
	int i;

	command_init(&command);
	command.index = index;

	// review this latter and include find_path logic
	if (tokens[0][0] != '<')
		command.name = ft_strdup(tokens[0]);
	else
		command.name = ft_strdup(tokens[2]);
	i = 0;
	while (tokens[i])
	{
		if (!ft_strncmp(tokens[i], "|", 1))
		{
			init_pipe(&command, tokens, &i, &index);
			break ;
		}
		else
			check_operators(&command, tokens, i);
		i++;
	}
	return (command);
}


void	parser(char *line, t_command *command)
{
	char	**tokens;

	tokens = NULL;
	tokens = lexer(line, &tokens);
	*command = analyser(tokens, 0);
	free_arr((void **)tokens);
}
