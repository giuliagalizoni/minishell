/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:06:25 by ggalizon          #+#    #+#             */
/*   Updated: 2025/05/20 15:08:24 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
