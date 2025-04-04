#include "includes/minishell.h"

t_command	analyser(char **tokens, int index);

void	init_pipe(t_command *command, char **tokens, int *i, int *index)
{
	command->is_pipe = 1;
	tokens++;
	command->pipe_next = malloc(sizeof(t_command));
	if (!command->pipe_next)
		return ;
	*command->pipe_next = analyser(tokens + ((*i)++), (*index) + 1);
}

void    check_operators(t_command *command, char **tokens, int i)
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
// parser testing main
// int	main()
// {
// 	char *line = "< infile grep a1 | wc -w >> outfile";
// 	t_command command;
// 	parser(line, &command);

// 	t_command *current = &command;
// 	while (current)
// 	{
// 		printf("Command index: %d\n", current->index);
// 		printf("Command name: %s\n", current->name);
// 		current = current->pipe_next;
// 	}
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
// 		if (command.pipe_next->output_redirect)
//             printf("next.output_redirect: %s\n", command.pipe_next->output_redirect);
//         else
//             printf("next.output_redirect: (null)\n");
//         printf("heredoc_delimiter: %s\n", command.pipe_next->heredoc_delimiter);
//         printf("is_heredoc: %i\n", command.pipe_next->is_heredoc);
//         printf("append_output: %i\n", command.pipe_next->append_output);
//         printf("is_pipe: %i\n", command.pipe_next->is_pipe);
//     }

// 	if (command.arguments)
// 		free_arr((void **)command.arguments);
// }
