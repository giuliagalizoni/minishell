#include "includes/minishell.h"

volatile sig_atomic_t	g_signal_code;

static void	msh_init(t_msh *msh, char **envp)
{
	msh->command = NULL;
	msh->myenv = init_envp(envp);
	msh->exit = 0;
	msh->exit_status = 0;
	using_history();
	print_banner();
}

static void	parse_and_execute(t_msh *msh, char *line)
{
	msh->command = parser(line, msh);
	msh->num_cmds = count_commands(msh->command);
	process(msh);
	clear_command_chain(msh->command);
	msh->command = NULL;
	add_history(line);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_msh	msh;

	(void)argc;
	(void)argv;
	msh_init(&msh, envp);
	while (!msh.exit)
	{
		set_signals_parent();
		line = readline("🐚\033[38;5;199mconchinha\033[38;5;99m>\033[0m ");
		if (g_signal_code == SIGINT)
			msh.exit_status = 130;
		else if (g_signal_code == SIGQUIT)
			msh.exit_status = 131;
		g_signal_code = -1;
		set_signals_child();
		if (!line)
			exit_shell(&msh);
		if (ft_strlen(line) != 0)
			parse_and_execute(&msh, line);
		free(line);
	}
	return (exit_shell(&msh), 0);
}
