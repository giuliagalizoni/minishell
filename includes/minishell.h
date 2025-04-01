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

typedef struct s_command {
    char *name;               // Command name
    char **arguments;            // Arguments
    char *input_redirect;        // Input redirection file
    char *output_redirect;       // Output redirection file
    int append_output;           // Boolean for append mode >> or >
    int is_pipe;                 // Boolean: Is this command part of a pipe?
    struct s_command *next;      // Pointer to the next command in the sequence
} t_command;

// stuff copied directly from pipex
void	process(t_command *cmd);
// parser
char **get_tokens(char *line);
void analyser(char **tokens, t_command *command);
void	command_init(t_command *command);
// path_utils
char	**get_paths(char **envp);
char	*get_cmd_path(char *cmd, char **envp);
// array_utils
void	free_array(void **arr);
void	arr_push(char ***arr, char *str);

#endif
