#ifndef MINISHELL_H
# define MINISHELL_H
# include "../../libft/src/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>

typedef struct s_vars
{
	char	*key;
	char	*value;
	struct s_vars	*next;
} t_vars;

typedef struct	s_outfile
{
	int	is_append;
	char	*filename;
	struct s_outfile *next;
}	t_outfile;

typedef struct	s_command
{
	char	*name;               // Command name
	char	*path;
	char	**arguments;            // Arguments
	char	**input_redirect;        // Input redirection file
	int	is_heredoc;             // 1 if "<<", 0 otherwise
	char	*heredoc_delimiter;     // Delimiter for "<<"
//	char	**output_redirect;       // Output redirection file
//	int	append_output;           // Boolean for append mode >> or >
	t_outfile	*outfile;
	int	is_pipe;                 // Boolean: Is this command part of a pipe?
	struct	s_command *pipe_next; // |
	int	index;
}	t_command;

typedef struct s_msh
{
	t_vars **env;
	t_command **command;
} t_msh;

// path_utils
char	**get_paths(char **envp);
char	*get_cmd_path(char *cmd, char **envp);
// array_utils
void	free_arr(void **arr);
char	**arr_push(char ***arr, char *str);
// command_utils
void	command_init(t_command *command);
void	set_command_paths(t_command *command, char **envp);
void	clear_command_chain(t_command *command);
int	count_commands(t_command *command);
// executer
// TODO Maybe move the cmd_count to the t_command struct
//void	child_process(t_command *cmd, int cmd_count);
// TODO parent_process has too many args
void	child_process(t_command *cmd, int prev_pipe_read_fd, int *fd, int num_cmds);
void	parent_process(t_command *cmd, pid_t *pids, int pid, int *fd, int *prev_pipe_read_fd, int num_cmds);
void	process(t_command *cmd, int num_cmds, t_vars **exp_vars);
// parser
t_command	*parser(char *line, t_command *command, char **envp);
t_command	*analyser(char **tokens, int index, char **envp);
char	**lexer(char *line, char ***tokens);
// builtin_utils
int	is_builtin(char *name);
void	builtin_router(t_command *cmd, t_vars **exp_vars);

void	exit_shell(t_command *command);
void	echo(t_command *cmd);
void	export(t_command *cmd, t_vars **exp_vars);

//list_utils - maybe rename to redirection_utils or sthg like that later 
void	add_outfile(t_command *cmd, char **tokens, t_outfile **outfiles, int *i);
//general_utils
int	is_equal(char *str1, char *str2);

t_vars *init_envp(char **envp);

#endif
