/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliagalizoni <giuliagalizoni@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:06:25 by ggalizon          #+#    #+#             */
/*   Updated: 2025/05/22 23:22:13 by giuliagaliz      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static int	retokenize(char ***new_tokens, char *value)
// {
// 	char	**temp_tokens;
// 	int		i;
// 	int		success;

// 	temp_tokens = ft_split(value, ' ');
// 	if (!temp_tokens)
// 	{
// 		perror("ft_split failed or returned no words");
// 		return (0);
// 	}
// 	i = 0;
// 	success = 1;
// 	while (temp_tokens[i] && success)
// 	{
// 		success = safe_arr_push(new_tokens, temp_tokens[i]);
// 		i++;
// 	}
// 	free_arr((void **)temp_tokens);
// 	if (!success)
// 		return (0);
// 	return (1);
// }

static int	handle_single_quote(char *token, char ***new_tokens)
{
	char	*inner_content;
	char	*processed;
	int		success;

	success = 1;
	inner_content = ft_substr(token, 1, ft_strlen(token) - 2);
	if (!inner_content)
		return (perror("ft_substr failed removing quotes"), 0);
	processed = remove_quotes(inner_content);
	free(inner_content);
	if (!processed)
		return (perror("remove_quotes failed"), 0);
	success = safe_arr_push(new_tokens, processed);
	free(processed);
	return (success);
}

static int	handle_unquoted_var(char *token, t_msh *msh, char ***new_tokens)
{
	char	*key;
	char	*value;
	char	*remainder;
	char	*expanded_token;
	int		var_length;
	int		success;

	// Determine where the variable ends (e.g., `$VARtest`)
	var_length = 1; // Start after `$`
	while (token[var_length] && ft_isalnum(token[var_length]))
		var_length++;

	// Extract the variable key
	key = ft_substr(token, 1, var_length - 1); // Skip `$` and take only the key part
	if (!key)
		return (perror("failed creating key substring"), 0);

	// Extract the remainder (if any, e.g., `test` in `$VARtest`)
	remainder = ft_strdup(token + var_length);
	if (!remainder)
		return (free(key), perror("failed duplicating remainder"), 0);

	// Get the value of the variable
	value = get_var_value(msh->myenv, key);
	free(key);

	// Combine expanded value and remainder
	if (value)
		expanded_token = ft_strjoin(value, remainder);
	else
		expanded_token = ft_strdup(remainder);

	free(remainder);
	if (!expanded_token)
		return (perror("failed combining expanded token"), 0);

	// Push the expanded token to new_tokens
	success = safe_arr_push(new_tokens, expanded_token);
	free(expanded_token);

	return (success);
}



// static int	handle_unquoted_var(char *token, t_msh *msh, char ***new_tokens)
// {
// 	char	*key;
// 	char	*value;
// 	int		success;

// 	success = 1;
// 	key = ft_substr(token, 1, ft_strlen(token) - 1);
// 	if (!key)
// 		return (perror("failed creating key substring"), 0);
// 	value = get_var_value(msh->myenv, key);
// 	free(key);
// 	if (value)
// 		success = retokenize(new_tokens, value);
// 	return (success);
// }

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
			success = handle_unquoted_var(token, msh, new_tokens);
	}
	else
		success = handle_mixed_token(token, new_tokens, msh);
	return (success);
}
