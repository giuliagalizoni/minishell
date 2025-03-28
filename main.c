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
int	main(int argc, char **argv, char **envp) {
	char	*line;
	t_command	command;
	char	**tokens;

	command_init(&command);
	using_history();
	if (argc == 1)
	while (1)
	{
		line = readline("minishell> ");
		printf("%s\n", argv[0]);
		tokens = get_tokens(line);
		analyser(tokens, &command);
		command.name = get_cmd_path(command.name, envp);
		printf("command.name: %s\n", command.name);
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
