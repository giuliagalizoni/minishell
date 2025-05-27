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

static int	handle_mixed_token(char *token, char ***new_tokens, t_msh *msh)
{
	char	*expanded;
	char	*processed;
	int		success;

	if (is_equal(token, "$"))
		return (safe_arr_push(new_tokens, token));
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
