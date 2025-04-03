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
	t_command	*tmp;
	(void)argc;
	(void)argv;

	command_init(&command);
	using_history();
	while (1)
	{
		line = readline("minishell> ");
		parser(line, &command);
		set_command_paths(&command, envp);
		tmp = &command;
		while (tmp)
		{
			printf("Command: %s\n", tmp->name);
			tmp = tmp->pipe_next;
		}
		process(&command);
		clear_command_chain(&command);
		command_init(&command);

		/*
		command.name = get_cmd_path(command.name, envp);
		process(&command);

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
