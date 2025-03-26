#include "includes/minishell.h"

// int	main(int argc, char **argv, char **envp)
int main() {
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
  return (0);
}
