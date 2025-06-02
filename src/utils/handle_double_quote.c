/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_double_quote.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:02:25 by ggalizon          #+#    #+#             */
/*   Updated: 2025/06/02 13:02:26 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*remove_double_quotes(const char *str)
{
	size_t	i;
	size_t	j;
	char	*new_str;

	if (!str)
		return (NULL);
	new_str = malloc(ft_strlen(str) + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '"')
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

static char	*get_final_value(char *token_str, char *expanded_str)
{
	char	*result_val;

	if (is_shell_operator(expanded_str))
	{
		result_val = ft_strdup(token_str);
		if (!result_val)
			perror("ft_strdup for token failed");
	}
	else
	{
		result_val = remove_double_quotes(expanded_str);
		if (!result_val)
			perror("remove_double_quotes failed");
	}
	return (result_val);
}

int	handle_double_quote(char *token, char ***new_tokens, size_t len, t_msh *msh)
{
	char	*inner_content;
	char	*expanded;
	char	*final_value_to_push;
	int		success;

	inner_content = ft_substr(token, 1, len - 2);
	if (!inner_content)
		return (perror("ft_substr failed for inner_content"), 0);
	expanded = expand_inline(inner_content, msh);
	free(inner_content);
	if (!expanded)
		return (perror("expand_inline failed"), 0);
	final_value_to_push = get_final_value(token, expanded);
	if (!final_value_to_push)
		return (free(expanded), 0);
	success = safe_arr_push(new_tokens, final_value_to_push);
	return (free(final_value_to_push), free(expanded), success);
}
