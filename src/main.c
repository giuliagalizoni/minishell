#include "includes/minishell.h"

volatile sig_atomic_t	g_signal_code;

static void	msh_init(t_msh *msh, char **envp)
{
	msh->command = NULL;
	msh->myenv = init_envp(envp);
	if (!msh->myenv)
	{
		ft_putstr_fd("Error: Failer to initialize environment\n", 2);
		exit(EXIT_FAILURE);
	}
	msh->exit = 0;
	msh->exit_status = 0;
	msh->num_cmds = 0;
	using_history();
	print_banner();
}

int	contains_only_quotes(const char *str)
{
	int	single_count;
	int	double_count;
	int	i;

	single_count = 0;
	double_count = 0;
	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\'')
			single_count++;
		else if (str[i] == '\"')
			double_count++;
		else
			return (0);
		i++;
	}
	return ((single_count % 2 == 0) && (double_count % 2 == 0));
}

static void	parse_and_execute(t_msh *msh, char *line)
{
	if (contains_only_quotes(line))
	{
		ft_putstr_fd("conchinha: : command not found\n", 2);
		msh->exit_status = 127;
		return ;
	}
	if (parser(line, msh) != 0)
	{
		clear_command_chain(msh->command);
		msh->command = NULL;
		add_history(line);
		return ;
	}
	msh->num_cmds = count_commands(msh->command);
	if (!process(msh))
	{
		clear_command_chain(msh->command);
		msh->command = NULL;
		add_history(line);
		return ;
	}
	clear_command_chain(msh->command);
	msh->command = NULL;
	add_history(line);
}

static void	main_signal_reset(t_msh *msh)
{
	if (g_signal_code == SIGINT)
		msh->exit_status = 130;
	else if (g_signal_code == SIGQUIT)
		msh->exit_status = 131;
	g_signal_code = -1;
	set_signals_child();
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_msh	msh;
	int		exit_code;

	(void)argc;
	(void)argv;
	msh_init(&msh, envp);
	while (!msh.exit)
	{
		set_signals_parent();
		line = readline("🐚\033[38;5;199mconchinha\033[38;5;99m>\033[0m ");
		main_signal_reset(&msh);
		if (!line)
		{
			msh.exit = 1;
			break ;
		}
		if (ft_strlen(line) != 0)
			parse_and_execute(&msh, line);
		free(line);
	}
	exit_code = msh.exit_status;
	cleanup(&msh);
	exit(exit_code);
}
/*
int	main(int argc, char **argv)
{
	int	x;
	if (argc > 1)
	{
		printf("argv length: %d\n", array_length((void **)argv));
		if (!is_num_string(argv[1]))
		{
			printf("error\n");
			return (0);
		}
		x = ft_atoi(argv[1]) % 256;
		printf("%d\n", x);
	}
	return (0);
}
*/
