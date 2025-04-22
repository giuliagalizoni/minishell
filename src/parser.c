#include "includes/minishell.h"

static void	init_pipe(t_command *command, char **tokens, int *i, int *index, char **envp, t_msh *msh)
{
	command->is_pipe = 1;
	tokens++;
	command->pipe_next = analyser(tokens + ((*i)++), (*index) + 1, envp, msh);
}

static void	check_operators(t_command *command, char **tokens, int i, t_msh *msh)
{
	// change to isequal
	if (!ft_strncmp(tokens[i], "<<", 2))
		command->is_heredoc = 1;
	else if (!ft_strncmp(tokens[i], "<", 1))
		command->is_heredoc = 0;
	else if (i > 0 && !ft_strncmp(tokens[i-1], "<", 1))
		arr_push(&command->input_redirect, tokens[i]);
	else if (i > 0 && !ft_strncmp(tokens[i-1], "<<", 2))
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


t_command	*parser(char *line, t_msh *msh, char **envp)
{
	char	**tokens;

	tokens = NULL;
	tokens = lexer(line, &tokens);
	msh->command = analyser(tokens, 0, envp, msh);
	free_arr((void **)tokens);
	return (msh->command);
}
