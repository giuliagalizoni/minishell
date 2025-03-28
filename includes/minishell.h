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
char	*get_rawpath(char **envp);
char	*get_cmd_path(char *cmd, char *rawpath);
void	child_process(char **argv,
			char **envp, int *pipe_fd);
void	parent_process(char **argv,
			char **envp, int *pipe_fd);

#endif
