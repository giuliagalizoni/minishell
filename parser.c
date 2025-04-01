#include "includes/minishell.h"

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

// figure out how to create the next commands
t_command	analyser(char **tokens)
{
	t_command command;

	command_init(&command);
	//idk yet if this rule works for all the cases
	if (tokens[0][0] != '<')
		command.name = ft_strdup(tokens[0]);
	else
		command.name = ft_strdup(tokens[2]);
	while (*tokens) //I'm not completely sure if the use of strncmp is the best one for all cases, but for now it's working
	{
		if (!ft_strncmp(*tokens, "<<", 2)) //not heredoc, regular input redirection
			command.is_heredoc = 1;
		else if (!ft_strncmp(*tokens, "<", 1)) //heredoc
			command.is_heredoc = 0;
		else if (*(tokens-1) && !ft_strncmp(*(tokens-1), "<", 1)) // if the previous is redirection operator, the current will be infile
			command.input_redirect = ft_strdup(*tokens);
		else if (*(tokens-1) && !ft_strncmp(*(tokens-1), "<<", 2)) // if the previous is heredoc operator, the current will be heredoc delimiter
			command.heredoc_delimiter = ft_strdup(*tokens);
		else if (!ft_strncmp(*tokens, ">>", 2)) // just redirect, not append
			command.append_output = 1;
		else if (!ft_strncmp(*tokens, ">", 1)) // append
			command.append_output = 0;
		else if ((*tokens -1) && !ft_strncmp((*tokens-1), ">>", 2))
			command.output_redirect = ft_strdup(*tokens);
		else if (*(tokens-1) && !ft_strncmp(*(tokens-1), ">", 1))
			command.output_redirect = ft_strdup(*tokens);
		else if (!ft_strncmp(*tokens, "|", 1)) // pipe
		{
			command.is_pipe = 1;
			tokens++;
			command.pipe_next = malloc(sizeof(t_command));
			if (!command.pipe_next)
				return command;
			*command.pipe_next = analyser(tokens);
			break ;
		}
		else // all other cases will be arguments
			arr_push(&command.arguments, *tokens);
		tokens++;
	}
	return (command);
}


//idk if it's better to return the command or to receive it from main as a pointer and just change it here
void	parser(char *line, t_command *command)
{
	// t_command	command;
	char	**tokens;
	// command_init(command); // figure this out
	tokens = lexer(line);
	*command = analyser(tokens);
	free_arr(tokens);
}

// int	main()
// {
// 	char *line = "< infile grep a1 | wc -w >> outfile";
// 	t_command command;
// 	command_init(&command);
// 	parser(line, &command);
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
// 	if (command.pipe_next)
//     {
//         printf("pipe_next.name: %s\n", command.pipe_next->name);
//         i = 0;
//         while (command.pipe_next->arguments && command.pipe_next->arguments[i])
//         {
//             printf("next.arg[%d]: %s\n", i, command.pipe_next->arguments[i]);
//             i++;
//         }
//         if (command.pipe_next->input_redirect)
//             printf("next.input_redirect: %s\n", command.pipe_next->input_redirect);
//         else
//             printf("next.input_redirect: (null)\n");
//         printf("heredoc_delimiter: %s\n", command.pipe_next->heredoc_delimiter);
//         printf("is_heredoc: %i\n", command.pipe_next->is_heredoc);
//         printf("append_output: %i\n", command.pipe_next->append_output);
//         printf("is_pipe: %i\n", command.pipe_next->is_pipe);
//     }

// 	if (command.arguments)
// 		free_arr(command.arguments);
// }
