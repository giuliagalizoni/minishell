#include "includes/minishell.h"

char **get_tokens(char *line)
{
	char	**tokens;

	tokens = ft_split(line, ' '); //check if we need to deal with other kinds of space char
	if (!tokens)
		return (NULL);
	return tokens;
}

void arr_push(char ***arr, char *str)
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
	free(*arr);
	*arr = new_arr;
}

void analyser(char **tokens, t_command *command)
{
	int i;

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


// int	main()
// {
// 	t_command command;

// 	command_init(&command);
// 	char *line = "ls -l";
// 	char **tokens = get_tokens(line);
// 	analyser(tokens, &command);

// 	printf("Command name: %s\n", command.name);
// 	int i = 0;
// 	while(command.arguments[i])
// 	{
// 		printf("arg[%d]: %s\n", i, command.arguments[i]);
// 		i++;
// 	}
// }
