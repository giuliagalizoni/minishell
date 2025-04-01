#include "includes/minishell.h"

// int	main()
// {
//	t_command command;

//	command_init(&command);
//	char *line = "ls -l";
//	char **tokens = get_tokens(line);
//	analyser(tokens, &command);

//	printf("Command name: %s\n", command.name);
//	int i = 0;
//	while(command.arguments[i])
//	{
//		printf("arg[%d]: %s\n", i, command.arguments[i]);
//		i++;
//	}
// }
// int	main(int argc, char **argv, char **envp)
/*
int	main(int argc, char **argv, char **envp) {
	char	*line;
	t_command	command;
	char	**tokens;
	(void)argc;
	(void)argv;

	command_init(&command);
	using_history();
	while (1)
	{
		line = readline("minishell> ");
		tokens = get_tokens(line);
		analyser(tokens, &command);
		command.name = get_cmd_path(command.name, envp);
		process(&command);
		command_init(&command);

		if (!line) {
		      printf("\nExiting minishell\n");
		}
		add_history(line);
		free(line);
	}
	clear_history();
	return (0);
}
*/

int	main(int argc, char **argv, char **envp)
{
//	char	*line;
	t_command	command;
	(void)argc;
	(void)argv;

	command.name = ft_strdup(get_cmd_path("ls", envp));
	command.arguments = ft_split(ft_strdup("-al /home"), ' ');
	process(&command);
//	ft_lstclear((t_list *)&command);

	return (0);
}




