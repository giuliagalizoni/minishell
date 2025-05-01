#include "includes/minishell.h"

volatile sig_atomic_t	g_exit_status;

int	main(int argc, char **argv, char **envp) {
	char	*line;
	t_msh	msh;

	(void)argc;
	(void)argv;

	g_exit_status = 0;
	msh.command = NULL;
	msh.myenv = init_envp(envp);
	msh.exit = 0;
	using_history();
	print_banner();
	while (!msh.exit)
	{
		set_signals_parent();
		line = readline("\033[38;5;199mconchinha\033[38;5;99m>\033[0m ");
		set_signals_child();
		if (!line)
			exit_shell(&msh);
		if (ft_strlen(line) != 0)
		{
			msh.command = parser(line, &msh, envp);
			msh.num_cmds = count_commands(msh.command);
			process(&msh);
			clear_command_chain(msh.command);
			msh.command = NULL;
			add_history(line);
			free(line);
		}
	}
	exit_shell(&msh);
	return (0);
}
