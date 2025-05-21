/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:22:04 by ggalizon          #+#    #+#             */
/*   Updated: 2025/05/20 17:22:05 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*remove_quotes(const char *str)
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
		if (str[i] != '\'' && str[i] != '"')
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

int	handle_exit_status(t_msh *msh, char ***new_tokens)
{
	char	*exit_status_str;
	int		success;

	exit_status_str = ft_itoa(msh->exit_status);
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
	if (!key)
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
			success = handle_exit_status(msh, new_tokens);
		else
			success = process_quoted_var(content, msh, new_tokens);
	}
	else
		success = safe_arr_push(new_tokens, content);
	return (success);
}

int	handle_double_quote(char *token, char ***new_tokens, size_t len, t_msh *msh)
{
	char	*literal;
	char	*expanded;
	int		success;

	literal = ft_substr(token, 1, len - 2);
	if (!literal)
		return (perror("ft_substr failed removing quotes"), 0);
	expanded = expand_inline(literal, msh);
	free(literal);
	if (!expanded)
		return (perror("expand_variables_in_line failed"), 0);
	success = safe_arr_push(new_tokens, expanded);
	free(expanded);
	return (success);
}
