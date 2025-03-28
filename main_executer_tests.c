#include "includes/minishell.h"

int	main(int argc, char **argv, char **envp) {
  /*
  char		*line;
  t_command	test_cmd;

  using_history();
  while (1) {
    line = readline("minishell> ");
    if (!line) {
      printf("\nExiting minishell\n");
    }
    add_history(line);
    printf("Your line: %s\n", line);
    free(line);
  }
  clear_history();
  */

	if (argc == 1)
	{
		printf("%s\n", argv[0]);
		t_command test_cmd;
		char	*cmd_name;

		cmd_name = get_cmd_path("ls", envp);
		printf("Full ls path: %s\n", cmd_name);
		test_cmd.name = cmd_name;
		test_cmd.arguments = ft_split("ls -l /home", ' ');
		process(&test_cmd);
	}
	return (0);
}
