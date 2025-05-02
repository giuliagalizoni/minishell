#include "includes/minishell.h"

static int	safe_arr_push(char ***arr, const char *str)
{
	char	*copy;

	copy = ft_strdup(str);
	if (!copy)
	{
		perror("ft_strdup failed in safe_arr_push");
		return (0);
	}
	if (!arr_push(arr, copy))
	{
		perror("arr_push failed in safe_arr_push");
		free(copy);
		return (0);
	}
	free(copy);
	return (1);
}

static int	handle_exit_status(char ***new_tokens)
{
	char	*exit_status_str;
	int		success;

	exit_status_str = ft_itoa(g_exit_status);
	if (!exit_status_str)
	{
		perror("ft_itoa failed for exit status");
		return (0);
	}
	success = safe_arr_push(new_tokens, exit_status_str);
	free(exit_status_str);
	return (success);
}

static int	retokenize(char ***new_tokens, char *value)
{
	char	**temp_tokens;
	int		i;
	int		success;

	temp_tokens = ft_split(value, ' ');
	if (!temp_tokens)
	{
		perror("ft_split failed or returned no words");
		return (0);
	}
	i = 0;
	success = 1;
	while (temp_tokens[i] && success)
	{
		success = safe_arr_push(new_tokens, temp_tokens[i]);
		i++;
	}
	free_arr((void **)temp_tokens);
	if (!success)
		return (0);
	return (1);
}

// static int	handle_regular_var(char *token, t_msh *msh, char ***new_tokens)
// {
// 	char	*key;
// 	char	*value;
// 	int		success;

// 	success = 1;
// 	key = ft_substr(token, 1, ft_strlen(token) - 1);
// 	if (!key)
// 	{
// 		perror("ft_substr failed for key");
// 		return (0);
// 	}
// 	value = get_var_value(msh->myenv, key);
// 	free(key);
// 	if (value)
// 		success = retokenize(new_tokens, value);
// 	return (success);
// }

// static int	handle_non_var(char *token, char ***new_tokens)
// {
// 	if (!safe_arr_push(new_tokens, token))
// 		return (0);
// 	return (1);
// }

static int	process_quoted_var(char *content, t_msh *msh, char ***new_tokens)
{
	char	*key;
	char	*value;
	int		success;

	success = 1;
	key = ft_substr(content, 1, ft_strlen(content) - 1);
	if(!key)
		return (perror("ft_substr failed for key"), 0);
	value = get_var_value(msh->myenv, key);
	if (value)
		success = safe_arr_push(new_tokens, value);
	return (success);
}

static int	process_inner(char *content, t_msh *msh, char ***new_tokens)
{
	int		success;

	success = 1;
	if (content[0] == '$' && content[1] != '\0')
	{
		if (content[1] == '?')
			success = handle_exit_status(new_tokens);
		else
			success = process_quoted_var(content, msh, new_tokens);
	}
	else
		success = safe_arr_push(new_tokens, content);
	return (success);
}

static int	handle_double_quote(char *token, t_msh *msh, char ***new_tokens)
{
	char	*inner_content;
	int		success;
	size_t	len;

	success = 1;
	len = ft_strlen(token);
	inner_content = ft_substr(token, 1, len - 2);
	if (!inner_content)
		return (perror("ft_substr failed removing quotes"), 0);
	success = process_inner(inner_content, msh, new_tokens);
	free(inner_content);
	return (success);
}

static int	handle_unquoted_var(char *token, t_msh *msh, char ***new_tokens)
{
	char	*key;
	char	*value;
	int		success;

	success = 1;
	key = ft_substr(token, 1, ft_strlen(token) - 1);
	if (!key)
		return (perror("failed creating key substring"), 0);
	value = get_var_value(msh->myenv, key);
	free(key);
	if (value)
		success = retokenize(new_tokens, value);
	return (success);
}
static int	handle_single_quote(char *token, char ***new_tokens, size_t len)
{
	char	*literal;
	int		success;

	literal = ft_substr(token, 1, len - 2);
	if (!literal)
		return (perror("ft_substr failed removing single quotes"), 0);
	success = safe_arr_push(new_tokens, literal);
	free(literal);
	return (success);
}

static int	process_one_token(char *token, t_msh *msh, char ***new_tokens)
{
	size_t	len;
	int		success;

	len = ft_strlen(token);
	success = 1;
	if (len >= 2 && token[0] == '\'' && token[len -1] == '\'')
		success = handle_single_quote(token, new_tokens, len);
	else if (len >= 2 && token[0] == '"' && token[len - 1] == '"')
		success = handle_double_quote(token, msh, new_tokens);
	else if (token[0] == '$' && token[1] != '\0')
	{
		if (token[1] == '?')
			success = handle_exit_status(new_tokens);
		else
			success = handle_unquoted_var(token, msh, new_tokens);
	}
	else
		success = safe_arr_push(new_tokens, token);
	return (success);
}

char	**expand_and_retokenize(char **tokens, t_msh *msh)
{
	char	**new_tokens;
	int		i;
	int		success;
	// size_t	len;

	new_tokens = NULL;
	i = 0;
	success = 1;
	while (tokens[i] && success)
	{
		success = process_one_token(tokens[i], msh, &new_tokens);
		if (!success)
			break ;
		i++;
	}
	if (!success)
		return (free_arr((void **)new_tokens), NULL);
	return (new_tokens);
}

// char	**expand_and_retokenize(char **tokens, t_msh *msh)
// {
// 	char	**new_tokens;
// 	int		i;
// 	int		success;

// 	new_tokens = NULL;
// 	i = 0;
// 	// TODO i get a segfault here if the line input to the shell is empty
// 	// I imagine that has to be caught further upstream tho
// 	while (tokens[i])
// 	{
// 		if (tokens[i][0] == '$' && tokens[i][1] != '\0')
// 		{
// 			if (tokens[i][1] == '?')
// 				success = handle_exit_status(msh, &new_tokens);
// 			else
// 				success = handle_regular_var(tokens[i], msh, &new_tokens);
// 		}
// 		else
// 			success = handle_non_var(tokens[i], &new_tokens);
// 		i++;
// 	}
// 	if (!success)
// 	{
// 		free_arr((void **)new_tokens);
// 		return (NULL);
// 	}
// 	return (new_tokens);
// }
