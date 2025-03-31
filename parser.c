#include "includes/minishell.h"

void	free_arr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	arr = NULL;
}
char	**lexer(char *line)
{
	char	**tokens;
	// ingnore '\' char
	// dealing with "" and '' - when there's a quote, we should not discard the space

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (NULL);
	return tokens;
}

void	arr_push(char ***arr, char *str)
{
	char	**new_arr;
	int	len;
	int	i;

	len = 0;
	while(*arr && (*arr)[len])
			len++;
	new_arr = (char **)malloc(sizeof(char *) * (len + 2)); //null term + new str
	if (!new_arr)
		return ;
	i = 0;
	while (i < len)
	{
		new_arr[i] = (*arr)[i];
		i++;
	}
	new_arr[i] = ft_strdup(str);
	if (!new_arr[i])
		return ;
	new_arr[i + 1] = NULL;
	if (*arr)
		free(*arr);
	*arr = new_arr;
}

// figure out how to create the next commands
void	analyser(char **tokens, t_command *command)
{
	int i;

	//idk yet if this rule works for all the cases
	if (tokens[0][0] != '<')
		command->name = ft_strdup(tokens[0]);
	else
		command->name = ft_strdup(tokens[2]);
	i = 0;
	while (tokens[i]) //I'm not completely sure if the use of strncmp is the best one for all cases, but for now it's working
	{
		if (!ft_strncmp(tokens[i], "<<", 2)) //not heredoc, regular input redirection
			command->is_heredoc = 1;
		else if (!ft_strncmp(tokens[i], "<", 1)) //heredoc
			command->is_heredoc = 0;
		else if (tokens[i-1] && !ft_strncmp(tokens[i-1], "<", 1)) // if the previous is redirection operator, the current will be infile
			command->input_redirect = ft_strdup(tokens[i]);
		else if (tokens[i-1] && !ft_strncmp(tokens[i-1], "<<", 2)) // if the previous is heredoc operator, the current will be heredoc delimiter
			command->heredoc_delimiter = ft_strdup(tokens[i]);
		else if (!ft_strncmp(tokens[i], ">>", 2)) // just redirect, not append
			command->append_output = 1;
		else if (!ft_strncmp(tokens[i], ">", 1)) // append
			command->append_output = 0;
		else if (tokens[i-1] && !ft_strncmp(tokens[i-1], ">>", 2))
			command->output_redirect = ft_strdup(tokens[i]);
		else if (tokens[i-1] && !ft_strncmp(tokens[i-1], ">", 1))
			command->output_redirect = ft_strdup(tokens[i]);
		else if (!ft_strncmp(tokens[i], "|", 1)) // pipe
		{
			command->is_pipe = 1;
			//here I guess we need to break the loop and set next pipe, somehow trigger the whole function again. I have an idea to do this recursively but maybe it's crazy
		}
		else if (!ft_strncmp(tokens[i], ";", 1))
		{
			//here we also need to break the loop and set next
		}
		else // all other cases will be arguments
			arr_push(&command->arguments, tokens[i]);
		i++;
	}
}

void	command_init(t_command *command)
{
	command->name = NULL;
	command->arguments = NULL;
	command->input_redirect = NULL;
	command->is_heredoc = 0;
	command->heredoc_delimiter = NULL;
	command->output_redirect = NULL;
	command->append_output = 0;
	command->is_pipe = -1;
	command->next = NULL;
	command->pipe_next = NULL;
}

//idk if it's better to return the command or to receive it from main as a pointer and just change it here
t_command	parser(char *line)
{
	t_command	command;
	char	**tokens;

	command_init(&command);
	tokens = lexer(line);
	analyser(tokens, &command);
	free_arr(tokens);
	return (command);
}

// int	main()
// {
// 	char *line = "< infile grep a1 | wc -w >> outfile";
// 	t_command command = parser(line);
// 	printf("Command name: %s\n", command.name);
// 	int i = 0;
// 	while(command.arguments[i])
// 	{
// 		printf("arg[%d]: %s\n", i, command.arguments[i]);
// 		i++;
// 	}
// 	printf("input_redirect: %s\n", command.input_redirect);
// 	printf("heredoc_delimiter: %s\n", command.heredoc_delimiter);
// 	printf("is_heredoc: %i\n", command.is_heredoc);
// 	printf("append_output: %i\n", command.append_output);
// 	printf("is_pipe: %i\n", command.is_pipe);

// 	if (command.arguments)
// 		free_arr(command.arguments);
// }
