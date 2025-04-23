#include "includes/minishell.h"

int	main(int argc, char **argv, char **envp) {
	char	*line;
	t_msh	msh;
	(void)argc;
	(void)argv;

	msh.command = NULL;
	msh.myenv = init_envp(envp);
	msh.exit_status = 0;
	using_history();
	print_banner();
	while (1)
	{
		line = readline("\033[38;5;199mconchinha\033[38;5;99m>\033[0m");
		msh.command = parser(line, &msh, envp);
		// set_command_paths(command, envp);
		msh.num_cmds = count_commands(msh.command);
		msh.exit_status = process(&msh);
		printf("exited with status %d\n", msh.exit_status);
		clear_command_chain(msh.command);
		if (!line)
		      printf("\nExiting minishell\n");
		add_history(line);
		free(line);
	}
	// free(command);
	clear_history();
	return (0);
}

/*
void	output_test()
{
	int	file;

	file = open("testytest", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	dup2(file, 1);
	close(file);
}

int	main()
{
	output_test();
	printf("testy\n");
	return (0);
}
*/
