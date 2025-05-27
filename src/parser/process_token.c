#include "../includes/minishell.h"

static int	handle_single_quote(char *token, char ***new_tokens)
{
	char	*inner_content;
	char	*final_value_to_push;
	int		success;

	inner_content = ft_substr(token, 1, ft_strlen(token) - 2);
	if (!inner_content)
		return (perror("ft_substr failed for single_quote inner_content"), 0);
	if (ft_strchr(inner_content, '\''))
	{
		final_value_to_push = remove_quotes(inner_content);
		if (!final_value_to_push)
			return (free(inner_content), perror("remove_quotes failed"), 0);
	}
	else
	{
		final_value_to_push = ft_strdup(inner_content);
		if (!final_value_to_push)
			return (free(inner_content),
				perror("ft_strdup of inner_content failed"), 0);
	}
	free(inner_content);
	success = safe_arr_push(new_tokens, final_value_to_push);
	free(final_value_to_push);
	return (success);
}

static int	extract_key_and_remainder(char *token, char **key, char **remainder)
{
	int	var_length;

	var_length = 1;
	while (token[var_length] && ft_isalnum(token[var_length]))
		var_length++;
	*key = ft_substr(token, 1, var_length - 1);
	if (!(*key))
		return (perror("failed creating key substring"), 0);
	*remainder = ft_strdup(token + var_length);
	if (!(*remainder))
	{
		free(*key);
		return (perror("failed duplicating remainder"), 0);
	}
	return (1);
}

static char	*expand_and_combine(char *key, char *remainder, t_msh *msh)
{
	char	*value;
	char	*combined;

	value = get_var_value(msh->myenv, key);
	free(key);
	if (value)
		combined = ft_strjoin(value, remainder);
	else
		combined = ft_strdup(remainder);
	free(remainder);
	return (combined);
}

static int	push_split_tokens(char *combined, char ***new_tokens)
{
	char	**split_value;
	int		success;
	int		i;

	split_value = ft_split(combined, ' ');
	if (!split_value)
		return (perror("failed splitting expanded token"), 0);
	success = 1;
	i = 0;
	while (split_value[i] && success)
	{
		success = safe_arr_push(new_tokens, split_value[i]);
		i++;
	}
	free_arr((void **)split_value);
	return (success);
}

static int	handle_unquoted_var(char *token, t_msh *msh, char ***new_tokens)
{
	char	*key;
	char	*remainder;
	char	*combined;
	int		success;

	if (!extract_key_and_remainder(token, &key, &remainder))
		return (0);
	combined = expand_and_combine(key, remainder, msh);
	if (!combined)
		return (perror("failed combining expanded token"), 0);
	success = push_split_tokens(combined, new_tokens);
	free(combined);
	return (success);
}

static int	handle_mixed_token(char *token, char ***new_tokens, t_msh *msh)
{
	char	*expanded;
	char	*processed;
	int		success;

	if (is_equal(token, "$"))
		return safe_arr_push(new_tokens, token);
	if (ft_strchr(token, '$'))
	{
		expanded = expand_inline(token, msh);
		if (!expanded)
			return (perror("variable expansion failed"), 0);
	}
	else
		expanded = token;
	processed = remove_quotes(expanded);
	if (!processed)
		return (perror("remove_quotes failed"), 0);
	success = safe_arr_push(new_tokens, processed);
	free(processed);
	return (success);
}

int	process_one_token(char *token, t_msh *msh, char ***new_tokens)
{
	size_t	len;
	int		success;

	len = ft_strlen(token);
	success = 1;
	if (len >= 2 && token[0] == '\'' && token[len -1] == '\'')
		success = handle_single_quote(token, new_tokens);
	else if (len >= 2 && token[0] == '"' && token[len - 1] == '"')
		success = handle_double_quote(token, new_tokens, len, msh);
	else if (token[0] == '$' && token[1] != '\0')
	{
		if (token[1] == '?')
			success = handle_exit_status(msh, new_tokens);
		else
		{
			if (ft_strchr(token + 1, '$'))
				success = handle_mixed_token(token, new_tokens, msh);
			else
				success = handle_unquoted_var(token, msh, new_tokens);
		}
	}
	else
		success = handle_mixed_token(token, new_tokens, msh);
	return (success);
}
