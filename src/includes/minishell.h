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
# include <sys/stat.h>
# define BANNER_FILE_PATH "./cfg/banner.txt"
# define PATH_MAX 4096

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
	int					heredoc_fd[2];
	int					heredoc_is_final;
	t_outfile			*outfile;
	int					is_pipe;
	struct s_command	*pipe_next;
	int					index;
	pid_t				pid;
}	t_command;

typedef struct s_msh
{
	t_vars		*myenv;
	t_command	*command;
	int			num_cmds;
	int			exit;
	int			exit_status;
}	t_msh;

//extern volatile sig_atomic_t g_exit_status;
extern volatile sig_atomic_t	g_signal_code;

// path_utils
char		**get_paths(t_vars *myenv);
char		*get_cmd_path(char *cmd, t_vars *myenv);
// array_utils
void		free_arr(void **arr);
char		**arr_push(char ***arr, char *str);
char		**myenv_to_envp(t_vars *myenv);
// command_utils
void		command_init(t_command *command);
void		set_command_paths(t_command *command, char **envp);
void		clear_command_chain(t_command *command);
int			count_commands(t_command *command);
int			is_directory(const char *path);
// error_utils
void		error_cleanup(t_msh *msh);
void		exit_process(t_msh *msh, t_command *command, char *arg,
				char *err_msg);
void		export_error(char *arg);
int			ft_perror(char *cmd_name, char *arg, int status, char *err_msg);
int			invalid_option_error(t_command *cmd, char *arg, int status);

// executer
void		child_process(t_msh *msh, t_command *command,
				int prev_pipe_read_fd, int *fd);
int	single_parent_process(t_msh *msh);
void		parent_process(t_msh *msh, t_command *command,
				int *fd, int *prev_pipe_read_fd);
int			process(t_msh *msh);
// redirection
int			input_redirection(t_command *command);
int			output_redirection(t_outfile *outfile);

// parser
int			parser(char *line, t_msh *msh);
t_command	*analyser(char **tokens, int index, t_msh *msh);
int			is_shell_operator(char *s);

// lexer
int			lexer(char *line, char ***tokens);
int			process_token(char *line, int *i, char ***tokens, char *result);
void		push_token(char ***tokens, char *result);

// builtin_utils
int			is_builtin(char *name);
int			builtin_router(t_msh *msh, t_command *command);
void		child_builtin(t_msh *msh, t_command *command);

// builtins
void		exit_shell(t_msh *msh, t_command *command);
int			echo(t_command *cmd);
int			export(t_msh *msh, t_command *command);
int			update_var_value(t_vars *var, const char *value);
int			env(t_msh *msh, t_command *command);
int			cd(t_command *command, t_vars *myenv);
int			pwd(t_command *command);

//cleanup_utils
void		cleanup(t_msh *msh);
void		clean_myenv(t_vars *myenv);

//list_utils
int			add_outfile(char **tokens, t_outfile **outfiles, int *i);
void		sort_vars_list(t_vars *head);

//general_utils
int			is_equal(char *str1, char *str2);
char		*ft_strncat(char *dest, const char *src, size_t n);
int			p_syntax_error(char *token);
int			ft_strcmp(char *s1, char *s2);
char		*ft_triplestrjoin(char *str1, char *str2, char *str3);
int			is_num_string(char *str);
int			array_length(void **arr);

//startup
void		print_banner(void);

//signals
void		set_signals_parent(void);
void		set_signals_child(void);
void		reset_prompt(int signal);
void		signal_newline(int signal);
void		sig_ignore(void);
void		set_signals_heredoc(void);

//env
t_vars		*init_envp(char **envp);
//unset
int			unset(t_msh *msh, t_command *command);
//expand var
char		*get_var_value(t_vars *head, char *key);
char		**expand_and_retokenize(char **tokens, t_msh *msh);
int			process_one_token(char *token, t_msh *msh, char ***new_tokens);
//expand var utils
int			safe_arr_push(char ***arr, const char *str);
int			handle_exit_status(t_msh *msh, char ***new_tokens);
int			process_quoted_var(char *content, t_msh *msh, char ***new_tokens);
int			process_inner(char *content, t_msh *msh, char ***new_tokens);
int			handle_double_quote(char *token, char ***new_tokens,
				size_t len, t_msh *msh);
//check syntax
int			check_invalid_syntax(char **tokens);

// export
t_vars		*find_last(t_vars *lst);
void		push_list(t_vars **exp_vars, t_vars *new);
void		print_vars(t_vars *vars);
void		*handle_malloc_error(char **key, char **value, char *str);
int			validate_key(char	*key);
t_vars		*parse_var(char *arg);

// heredoc
int			handle_heredoc(t_command *command, t_msh *msh);
int			process_heredocs(t_msh *msh);
int			process_expansion(char *line, int *i, t_msh *msh, char ***parts);
int			process_line(char *line, t_msh *msh, char ***parts);

//expand inline
char		*expand_inline(char *line, t_msh *msh);
char		*remove_quotes(const char *str);
void		toggle_quote(char *quote, char line_char);

//process tokens utils
int			handle_unquoted_var(char *token, t_msh *msh, char ***new_tokens);
#endif
