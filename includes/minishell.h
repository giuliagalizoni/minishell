#ifndef MINISHELL_H
#define MINISHELL_H
#include "../libft/src/libft.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
# include <fcntl.h>

// stuff copied directly from pipex
void	process(t_command *cmd);
char	**get_paths(char **envp);
char	*get_cmd_path(char *cmd, char **envp);

#endif
