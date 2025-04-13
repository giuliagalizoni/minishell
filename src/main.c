#include "includes/minishell.h"

int	main(int argc, char **argv, char **envp) {
	char	*line;
	t_command	*command;
	int	num_cmds;
	(void)argc;
	(void)argv;

	command = malloc(sizeof(t_command));
	if (!command)
		return (1);
	command_init(command);
	free(command);
	using_history();
	while (1)
	{
		line = readline("minishell> ");

		command = parser(line, command, envp);
		// set_command_paths(command, envp);
		num_cmds = count_commands(command);
		process(command, num_cmds);
		clear_command_chain(command);
		if (!line)
		      printf("\nExiting minishell\n");
		add_history(line);
		free(line);
	}
	// free(command);
	clear_history();
	return (0);
}
