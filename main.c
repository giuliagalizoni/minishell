#include "includes/minishell.h"

int	main(int argc, char **argv, char **envp) {
	char	*line;
	t_command	command;
	int	num_cmds;
	(void)argc;
	(void)argv;

	command_init(&command);
	using_history();
	while (1)
	{
		line = readline("minishell> ");
		parser(line, &command);
		set_command_paths(&command, envp);
		num_cmds = count_commands(&command);
		process(&command, num_cmds);
		clear_command_chain(&command);
		command_init(&command);
		if (!line)
		      printf("\nExiting minishell\n");
		add_history(line);
		free(line);
	}
	clear_history();
	return (0);
}
