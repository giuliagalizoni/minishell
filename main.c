#include "includes/minishell.h"

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
