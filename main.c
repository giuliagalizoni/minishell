#include "includes/minishell.h"

/*
int	main(int argc, char **argv, char **envp) {
	char	*line;
	t_command	command;
	(void)argc;
	(void)argv;

	command_init(&command);
	using_history();
	while (1)
	{
		line = readline("minishell> ");
		parser(line, &command);
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
int	main(int argc, char **argv, char **envp) {
	char	*line;
	t_command	command;
	(void)argc;
	(void)argv;

	command_init(&command);
	using_history();
	while (1)
	{
		line = readline("minishell> ");
		parser(line, &command);
		while (command.pipe_next)
		{
			printf("Command: %s\n", command.name);
			command = *command.pipe_next;
		}
		/*
		command.name = get_cmd_path(command.name, envp);
		process(&command);
		command_init(&command);

		if (!line) {
		      printf("\nExiting minishell\n");
		}
		*/
		add_history(line);
		free(line);
	}
	clear_history();
	return (0);
}
