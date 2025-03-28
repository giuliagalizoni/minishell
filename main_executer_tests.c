#include "includes/minishell.h"

int	main() {
	/*
  char *line;

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

	t_command test_cmd;
	test_cmd.name = "/usr/bin/ls";
	test_cmd.arguments = "ls -l /home";
	process(cmd);
	return (0);
}
