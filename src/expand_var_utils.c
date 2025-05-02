#include "includes/minishell.h"

int	safe_arr_push(char ***arr, const char *str)
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

int	handle_exit_status(char ***new_tokens)
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

int	process_quoted_var(char *content, t_msh *msh, char ***new_tokens)
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

int	process_inner(char *content, t_msh *msh, char ***new_tokens)
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

int	handle_single_quote(char *token, char ***new_tokens, size_t len)
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
