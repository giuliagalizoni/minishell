#include "includes/minishell.h"

volatile sig_atomic_t	g_exit_code;

void	sigint_handler(int signal)
{
	//TODO need to 
	if (signal == SIGINT)
	{
		g_exit_code = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_signal_action(void)
{
	struct sigaction sa_int;

	ft_bzero(&sa_int, sizeof(sa_int));
	sa_int.sa_handler = &sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	// TODO wrap up in a in if sigaction == -1 for error check
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int argc, char **argv, char **envp) {
	char	*line;
	t_msh	msh;

	(void)argc;
	(void)argv;

	g_exit_code = 0;
	set_signal_action();
	msh.command = NULL;
	msh.myenv = init_envp(envp);
	msh.exit_status = 0;
	using_history();
	print_banner();
	while (1)
	{
		line = readline("\033[38;5;199mconchinha\033[38;5;99m>\033[0m ");
		if (!line)
		{
			printf("bye\n");
			exit_shell(&msh);
		}
		if (ft_strlen(line) != 0)
		{
			g_exit_code = 0;
			msh.command = parser(line, &msh, envp);
			// set_command_paths(command, envp);
			msh.num_cmds = count_commands(msh.command);
			msh.exit_status = process(&msh);
			clear_command_chain(msh.command);
			add_history(line);
			free(line);
		}
	}
	// free(command);
	clear_history();
	return (0);
}
