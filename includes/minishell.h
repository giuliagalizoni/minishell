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

typedef struct s_command {
    char *name;               // Command name
    char **arguments;            // Arguments
    char *input_redirect;        // Input redirection file
    char *output_redirect;       // Output redirection file
    int append_output;           // Boolean for append mode >> or >
    int is_pipe;                 // Boolean: Is this command part of a pipe?
    struct s_command *next;      // Pointer to the next command in the sequence
} t_command;

#endif
