#include "includes/minishell.h"

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
			success = handle_exit_status(msh, new_tokens);
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
