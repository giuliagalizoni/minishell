#ifndef MINISHELL_H
# define MINISHELL_H
# include "../../libft/src/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# define BANNER_FILE_PATH "./cfg/banner.txt"

typedef struct s_vars
{
	char			*key;
	char			*value;
	struct s_vars	*next;
}	t_vars;

typedef struct s_outfile
{
	int					is_append;
	char				*filename;
	struct s_outfile	*next;
}	t_outfile;

typedef struct s_command
{
	char				*name;
	char				*path;
	char				**arguments;
	char				**input_redirect;
	int					is_heredoc;
	char				*heredoc_delimiter;
	int					heredoc_fd;
	t_outfile			*outfile;
	int					is_pipe;
	struct s_command	*pipe_next;
	int					index;
	pid_t				pid;
}	t_command;

typedef struct s_msh
{
	t_vars		*myenv;
	char		**env;
	t_command	*command;
	int			num_cmds;
	int			exit;
	int			exit_status;
}	t_msh;

//extern volatile sig_atomic_t g_exit_status;

// path_utils
char		**get_paths(char **envp);
char		*get_cmd_path(char *cmd, char **envp);
// array_utils
void		free_arr(void **arr);
char		**arr_push(char ***arr, char *str);
// command_utils
void		command_init(t_command *command);
void		set_command_paths(t_command *command, char **envp);
void		clear_command_chain(t_command *command);
int			count_commands(t_command *command);
// executer
// TODO Maybe move the cmd_count to the t_command struct
//void	child_process(t_command *cmd, int cmd_count);
// TODO parent_process has too many args
void		child_process(t_msh *msh, int prev_pipe_read_fd, int *fd);
void		parent_process(t_msh *msh, int *fd, int *prev_pipe_read_fd);
int			process(t_msh *msh);
// parser
t_command	*parser(char *line, t_msh *msh);
t_command	*analyser(char **tokens, int index, t_msh *msh);
// lexer
int			lexer(char *line, char ***tokens);
int			process_token(char *line, int *i, char ***tokens, char *result);
void		push_token(char ***tokens, char *result);

// builtin_utils
int			is_builtin(char *name);
int			builtin_router(t_msh *msg);
void		child_builtin(t_msh *msh);

void		exit_shell(t_msh *msh);
void		echo(t_command *cmd);
void		export(t_msh *msh);
int			cd(t_command *command);
int			pwd(void);

//list_utils - maybe rename to redirection_utils or sthg like that later
void		add_outfile(t_command *cmd, char **tokens,
					t_outfile **outfiles, int *i);
void		sort_vars_list(t_vars *head);

//general_utils
int			is_equal(char *str1, char *str2);
char		*ft_strncat(char *dest, const char *src, size_t n);
int			p_syntax_error(char *token);
int			ft_strcmp(char *s1, char *s2);

//startup
void		print_banner(void);

//signals
void		set_signals_parent(void);
void		set_signals_child(void);
void		reset_prompt(int signal);
void		signal_newline(int signal);
void		sig_ignore(void);

//env
t_vars		*init_envp(char **envp);
void		clean_myenv(t_vars *myenv);
void		print_env(t_vars *myenv);

//unset
void		unset(t_msh *msh);

//expand var
char		*get_var_value(t_vars *head, char *key);
char		**expand_and_retokenize(char **tokens, t_msh *msh);
//expand var utils
int			safe_arr_push(char ***arr, const char *str);
int			handle_exit_status(char ***new_tokens);
int			process_quoted_var(char *content, t_msh *msh, char ***new_tokens);
int			process_inner(char *content, t_msh *msh, char ***new_tokens);
int			handle_single_quote(char *token, char ***new_tokens, size_t len);
//check syntax
int			check_invalid_syntax(char **tokens);

// export
t_vars		*find_last(t_vars *lst);
void		push_list(t_vars **exp_vars, t_vars *new);
void		print_vars(t_vars *vars);
void		*handle_malloc_error(char **key, char **value, char *str);
int			validate_key(char	*key);

// heredoc
void		handle_heredoc(t_command *command);
void		process_heredocs(t_msh *msh);

#endif
