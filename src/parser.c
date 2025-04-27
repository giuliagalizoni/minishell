#include "includes/minishell.h"

static void	init_pipe(t_command *command, char **tokens, int *i, int *index, char **envp, t_msh *msh)
{
	command->is_pipe = 1;
	tokens++;
	command->pipe_next = analyser(tokens + ((*i)++), (*index) + 1, envp, msh);
}

static void	check_operators(t_command *command,
	char **tokens, int i, t_msh *msh)
{
	if (is_equal(tokens[i], "<<"))
		command->is_heredoc = 1;
	else if (is_equal(tokens[i], "<"))
		command->is_heredoc = 0;
	else if (i > 0 && is_equal(tokens[i-1], "<"))
		arr_push(&command->input_redirect, tokens[i]);
	else if (i > 0 && is_equal(tokens[i-1], "<<"))
		command->heredoc_delimiter = ft_strdup(tokens[i]);
	/*
	else if (!ft_strncmp(tokens[i], ">>", 2))
		command->append_output = 1;
	else if (!ft_strncmp(tokens[i], ">", 1))
		command->append_output = 0;
	else if (i > 0 && !ft_strncmp((tokens[i-1]), ">>", 2))
		arr_push(&command->output_redirect, tokens[i]);
	else if (i > 0 && !ft_strncmp(tokens[i-1], ">", 1))
		arr_push(&command->output_redirect, tokens[i]);
		*/
	else if (tokens[i][0] == '$')
	{
		char *key = ft_substr(tokens[i], 1, (ft_strlen(tokens[i])) - 1);
		// if key is ? do smth else
		// printf("argument %s will be key %s\n", tokens[i], key);
		// printf("key %s has value \"%s\"\n", key, get_var_value(msh->myenv, key));
		arr_push(&command->arguments, get_var_value(msh->myenv, key));
	}
	else
		arr_push(&command->arguments, tokens[i]);
}

void	set_name(t_command *command, char **tokens, char **envp)
{
	int i;
	// errors "unexpected token" like <<< or < < < > or
	i = 0;
	// need to also account for > and >>
	while(tokens[i][0] == '<')
		i = i + 2;
	command->name = ft_strdup(tokens[i]);
	command->path =	get_cmd_path(tokens[i], envp);
}

t_command	*analyser(char **tokens, int index, char **envp, t_msh *msh)
{
	t_command	*command;
	int i;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command_init(command);
	command->index = index;
	set_name(command, tokens, envp);
	i = 0;
	while (tokens[i])
	{
		if (!ft_strncmp(tokens[i], "|", 1))
		{
			init_pipe(command, tokens, &i, &index, envp, msh);
			break ;
		}
		else if (!ft_strncmp(tokens[i], ">", 1) || !(ft_strncmp(tokens[i], ">>", 2)))
			add_outfile(command, tokens, &command->outfile, &i);
		else
			check_operators(command, tokens, i, msh);
		i++;
	}
	return (command);
}

int is_redirection(char *token) {
	if (!token) return 0;
	return (is_equal(token, ">") || is_equal(token, "<") ||
			is_equal(token, ">>") || is_equal(token, "<<"));
}

int is_pipe(char *token) {
	if (!token) return 0;
	return (is_equal(token, "|"));
}

int is_operator(char *token) {
	if (!token) return 0; // Handle NULL token
	return (is_pipe(token) || is_redirection(token));
}
// maybe change printfs to write or code a special printf to print in stderr
static int check_invalid_syntax(char **tokens)
{
	int i;
	int last;

	if (!tokens || !tokens[0])
		return (1);
	if (is_operator(tokens[0]) && is_pipe(tokens[0]))
	{
		printf("minishell: syntax error near unexpected token `%s'\n", tokens[0]);
		return (0);
	}
	last = -1;
	while (tokens[last + 1])
		last++;
	if (last >= 0 && is_operator(tokens[last]))
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (0);
	}
	i = 0;
	while (tokens[i])
	{
		if (is_redirection(tokens[i]))
		{
			if (!tokens[i+1] || is_operator(tokens[i+1]))
			{
				printf("minishell: syntax error near unexpected token `%s'\n", tokens[i]);
				return (0);
			}
		}
		else if (is_pipe(tokens[i]))
		{
			if (!tokens[i+1] || is_pipe(tokens[i+1]))
			{
				printf("minishell: syntax error near unexpected token `%s'\n", tokens[i]);
				return (1);
			}
		}
		i++;
	}
	return (1);
}

t_command	*parser(char *line, t_msh *msh, char **envp)
{
	char	**tokens;
	char	**retokens;

	if (!line)
		return NULL;
	tokens = NULL;
	tokens = lexer(line, &tokens);
	// should the parser return an int so we can check the status code?
	//syntax error code 2
	if (!tokens)
		return NULL;
	if (tokens && !check_invalid_syntax(tokens))
	{
		free_arr((void **)tokens);
		return (NULL);
	}
	retokens = expand_and_retokenize(tokens, msh);
	msh->command = analyser(retokens, 0, envp, msh);
	free_arr((void **)tokens);
	return (msh->command);
}
