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
	new_arr[i + 1] = NULL;
	free_arr(*arr);
	*arr = new_arr;
}

void	analyser(char **tokens, t_command *command)
{
	int i;
	char *temp;

	command->name = tokens[0];
	// arr_push(&command.arguments, tokens[0]);
	i = 0;
	while (tokens[i])
	{
		arr_push(&command->arguments, tokens[i]);
		i++;
	}

}

void	command_init(t_command *command)
{
	command->name = NULL;
	command->arguments = NULL;
	command->input_redirect = NULL;
	command->output_redirect = NULL;
	command->append_output = -1;
	command->is_pipe = -1;
	command->next = NULL;
}

void	parser(char *line)
{
	t_command	command;
	char	**tokens;

	command_init(&command);
	tokens = lexer(line);
	analyser(tokens, &command);
	free_arr(tokens);
}

// int	main()
// {
// 	t_command command;

// 	command_init(&command);
// 	char *line = "ls -l";
// 	char **tokens = lexer(line);
// 	analyser(tokens, &command);

// 	printf("Command name: %s\n", command.name);
// 	int i = 0;
// 	while(command.arguments[i])
// 	{
// 		printf("arg[%d]: %s\n", i, command.arguments[i]);
// 		i++;
// 	}
// }
