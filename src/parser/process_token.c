/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliagalizoni <giuliagalizoni@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:06:25 by ggalizon          #+#    #+#             */
/*   Updated: 2025/05/24 15:19:34 by giuliagaliz      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static int	handle_single_quote(char *token, char ***new_tokens)
// {
// 	char	*inner_content;
// 	char	*processed;
// 	int		success;

// 	success = 1;
// 	inner_content = ft_substr(token, 1, ft_strlen(token) - 2);
// 	if (!inner_content)
// 		return (perror("ft_substr failed removing quotes"), 0);
// 	processed = remove_quotes(inner_content);
// 	free(inner_content);
// 	if (!processed)
// 		return (perror("remove_quotes failed"), 0);
// 	success = safe_arr_push(new_tokens, processed);
// 	free(processed);
// 	return (success);
// }

static int	handle_single_quote(char *token, char ***new_tokens)
{
    char	*inner_content;
    char	*final_value_to_push;
    int		success;

    // Extract the content within the outermost single quotes.
    inner_content = ft_substr(token, 1, ft_strlen(token) - 2);
    if (!inner_content)
        return (perror("ft_substr failed for single_quote inner_content"), 0);

    // Check if the extracted inner_content still contains single quotes.
    if (ft_strchr(inner_content, '\''))
    {
        // If inner_content has more single quotes, apply remove_quotes to flatten everything.
        // remove_quotes is expected to strip all single and double quotes.
        final_value_to_push = remove_quotes(inner_content);
        if (!final_value_to_push)
        {
            free(inner_content); // Free the result of ft_substr
            return (perror("remove_quotes failed"), 0);
        }
    }
    else
    {
        // If inner_content has no more single quotes (it might have double quotes or be plain),
        // use it as is. We need to ft_strdup it because inner_content will be freed.
        final_value_to_push = ft_strdup(inner_content);
        if (!final_value_to_push)
        {
            free(inner_content); // Free the result of ft_substr
            return (perror("ft_strdup of inner_content failed"), 0);
        }
    }

    // At this point, inner_content (from ft_substr) is no longer directly needed,
    // as its data is either processed by remove_quotes or duplicated by ft_strdup
    // into final_value_to_push.
    free(inner_content);

    success = safe_arr_push(new_tokens, final_value_to_push);
    // Assuming safe_arr_push makes its own copy of final_value_to_push.
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
	for (i = 0; split_value[i] && success; i++)
		success = safe_arr_push(new_tokens, split_value[i]);
	free_arr((void **)split_value);
	return (success);
}

static int	handle_unquoted_var(char *token, t_msh *msh, char ***new_tokens)
{
	char	*key;
	char	*remainder;
	char	*combined;
	int		success;

	// Extract key and remainder
	if (!extract_key_and_remainder(token, &key, &remainder))
		return (0);

	// Expand variable and combine with remainder
	combined = expand_and_combine(key, remainder, msh);
	if (!combined)
		return (perror("failed combining expanded token"), 0);

	// Push tokens to new_tokens
	success = push_split_tokens(combined, new_tokens);
	free(combined);

	return (success);
}

static int	handle_mixed_token(char *token, char ***new_tokens, t_msh *msh)
{
	char	*expanded;
	char	*processed;
	int		success;

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
