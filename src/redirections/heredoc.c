#include "../includes/minishell.h"

static char *join_strings(char **arr)
{
	char	*result;
	size_t	total_len;
	int		i;

	if (!arr || !arr[0])
		return (ft_strdup(""));
	total_len = 0;
	i = 0;
	while (arr[i])
	{
		total_len += ft_strlen(arr[i]);
		i++;
	}
	result = (char *)malloc(sizeof(char) * (total_len + 1));
	if (!result)
		return (NULL);
	result[0] = '\0';
	i = 0;
	while (arr[i])
	{
		ft_strlcat(result, arr[i], total_len + 1);
		i++;
	}
	return (result);
}
// static int	append_literal(char *line, int start, int end, char ***parts)
// {
// 	char *segment;

// 	if (end > start)
// 	{
// 		segment = ft_substr(line, start, end - start);
// 		if (!segment || !arr_push(parts, segment))
// 			return (free(segment), 0);
// 		free(segment);
// 	}
// 	return (1);
// }

// static int	process_line(char *line, t_msh *msh, char ***parts)
// {
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	while (line[i])
// 	{
// 		if (line[i] == '$')
//  		{
// 			if (!append_literal(line, j, i, parts)
// 				|| !process_expansion(line, &i, msh, parts))
// 				return (0);
// 			j = i;
// 		}
// 		else
// 			i++;
// 	}
// 	return (append_literal(line, j, i, parts));
// }

static char	*expand_heredoc(char *line, t_msh *msh)
{
	char	**parts;
	char	*expanded;
	int		i;
	int		j;

	parts = NULL;
	i = 0;
	j = 0;
	if (!process_line(line, msh, &parts))
		return (free((void **)parts), ft_strdup(line));
	expanded = join_strings(parts);
	free_arr((void **)parts);
	if (!expanded)
		return (ft_strdup(""));
	return (expanded);
}

void	handle_heredoc(t_command *command, t_msh *msh)
{
	int		pipe_fd[2];
	char	*line;
	char *expanded_line;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe error");
		exit(EXIT_FAILURE); // TODO handle exit status e
	}
	command->heredoc_fd = pipe_fd[0];
	while (1)
	{
		line = readline("> ");
		if (!line || is_equal(line, command->heredoc_delimiter))
		{
			free(line);
			break ;
		}
		expanded_line = expand_heredoc(line, msh);
		free(line);
		if (expanded_line)
		{
			write(pipe_fd[1], expanded_line, ft_strlen(expanded_line));
			write(pipe_fd[1], "\n", 1);
			free(expanded_line);
		}
	}
	close(pipe_fd[1]);
}

void	process_heredocs(t_msh *msh)
{
	t_command	*command;

	command = msh->command;
	while (command)
	{
		if (command->is_heredoc)
			handle_heredoc(command, msh);
		command = command->pipe_next;
	}
}
