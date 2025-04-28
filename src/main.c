#include "includes/minishell.h"

volatile sig_atomic_t	g_exit_status;

int	main(int argc, char **argv, char **envp) {
	char	*line;
	t_msh	msh;

	(void)argc;
	(void)argv;

	g_exit_status = 0;
	set_signal_action();
	msh.command = NULL;
	msh.myenv = init_envp(envp);
	using_history();
	print_banner();
	while (1)
	{
		//g_exit_status = 0;
		line = readline("\033[38;5;199mconchinha\033[38;5;99m>\033[0m ");
		if (!line)
		{
			printf("bye\n");
			exit_shell(&msh);
		}
		if (ft_strlen(line) != 0)
		{
			msh.command = parser(line, &msh, envp);
			msh.num_cmds = count_commands(msh.command);
			process(&msh);
			printf("wtf is up with %d\n", g_exit_status);
			clear_command_chain(msh.command);
			add_history(line);
			free(line);
		}
	}
	// free(command);
	clear_history();
	return (0);
}
