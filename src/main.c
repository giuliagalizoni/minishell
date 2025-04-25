#include "includes/minishell.h"


void	INTHandler(int sig)
{
	signal(sig, SIG_IGN);
	readline("\033[38;5;199mconchinha\033[38;5;99m>\033[0m ");
}

void	sigint_handler(int signal)
{
	if (signal == SIGINT)
		printf("\nsiguinting!\n");
}

void	set_signal_action(void)
{
	struct sigaction act;
	bzero(&act, sizeof(act));
	act.sa_handler = &sigint_handler;
	sigaction(SIGINT, &act, NULL);
}

int	main(int argc, char **argv, char **envp) {
//	char	*line;
	t_msh	msh;

	(void)argc;
	(void)argv;

	set_signal_action();
	msh.command = NULL;
	msh.myenv = init_envp(envp);
	msh.exit_status = 0;
	using_history();
	print_banner();
	while (1)
	{
		/*
		signal(SIGINT, INTHandler);
		line = readline("\033[38;5;199mconchinha\033[38;5;99m>\033[0m ");
		msh.command = parser(line, &msh, envp);
		// set_command_paths(command, envp);
		msh.num_cmds = count_commands(msh.command);
		msh.exit_status = process(&msh);
		clear_command_chain(msh.command);
		if (!line)
		      printf("\nExiting minishell\n");
		add_history(line);
		free(line);
		*/
	}
	// free(command);
	clear_history();
	return (0);
}
