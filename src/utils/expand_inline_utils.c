/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_inline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliagalizoni <giuliagalizoni@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:23:04 by ggalizon          #+#    #+#             */
/*   Updated: 2025/05/25 11:20:19 by giuliagaliz      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static int	append_literal(char *line, int start, int end, char ***parts, char quote)
// {
// 	char	*segment;

// 	if (end > start)
// 	{
// 		segment = ft_substr(line, start, end - start);
// 		if (!segment)
// 			return (0);
// 		// Trim spaces if unquoted
// 		if (quote != '\"')
// 			segment = ft_strtrim(segment, " ");
// 		if (!arr_push(parts, segment))
// 			return (free(segment), 0);
// 		free(segment);
// 	}
// 	return (1);
// }

static int	append_literal(char *line, int start, int end, char ***parts)
{
	char	*segment;

	if (end > start)
	{
		segment = ft_substr(line, start, end - start);
		if (!segment || !arr_push(parts, segment))
			return (free(segment), 0);
		free(segment);
	}
	return (1);
}

static int	append_exit_status(t_msh *msh, char ***parts)
{
	char	*value;

	value = ft_itoa(msh->exit_status);
	if (!value || !arr_push(parts, value))
		return (free(value), 0);
	free(value);
	return (1);
}

// static int	append_variable(char *line, int *i, t_msh *msh, char ***parts)
// {
// 	int		start;
// 	char	*key;
// 	char	*value;

// 	start = *i + 1;
// 	(*i)++;
// 	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
// 		(*i)++;
// 	key = ft_substr(line, start, *i - start);
// 	if (!key)
// 		return (0);
// 	value = get_var_value(msh->myenv, key);
// 	if (value)
// 	{
// 		if (!arr_push(parts, value))
// 			return (0);
// 	}
// 	return (1);
// }

int	process_expansion(char *line, int *i, t_msh *msh, char ***parts, char quote)
{
	char	*value;

	if (line[*i + 1] == '?')
	{
		if (!append_exit_status(msh, parts))
			return (0);
		(*i) += 2;
	}
	else if (line[*i + 1] && (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_'))
	{
		int start = *i + 1;
		while (line[*i + 1] && (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_'))
			(*i)++;
		char *key = ft_substr(line, start, *i - start + 1);
		if (!key)
			return (0);
		value = get_var_value(msh->myenv, key);
		free(key);

		if (value)
		{
			if (quote) // Handle quoted strings (single unit)
			{
				if (!arr_push(parts, value))
					return (0);
			}
			else // Handle unquoted strings (split by spaces)
			{
				char **split_values = ft_split(value, ' ');
				if (!split_values)
					return (0);
				for (int j = 0; split_values[j]; j++)
				{
					if (!arr_push(parts, split_values[j]))
					{
						free_arr((void **)split_values);
						return (0);
					}
				}
				free_arr((void **)split_values);
			}
		}
	}
	else
	{
		if (!arr_push(parts, "$"))
			return (0);
		(*i)++;
	}
	return (1);
}



// int	process_expansion(char *line, int *i, t_msh *msh, char ***parts)
// {
// 	if (line[*i + 1] == '?')
// 	{
// 		if (!append_exit_status(msh, parts))
// 			return (0);
// 		(*i) += 2;
// 	}
// 	else if (line[*i + 1] && (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_'))
// 	{
// 		if (!append_variable(line, i, msh, parts))
// 			return (0);
// 	}
// 	else
// 	{
// 		if (!arr_push(parts, "$"))
// 			return (0);
// 		(*i)++;
// 	}
// 	return (1);
// }

int	process_line(char *line, t_msh *msh, char ***parts)
{
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	quote = 0;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '\"')
			&& (!quote || quote == line[i]))
			toggle_quote(&quote, line[i]);

		if (line[i] == '=' && !quote) // Preserve key=value structure
		{
			i++;
			continue;
		}

		if (line[i] == '$' && quote != '\'')
		{
			if (!append_literal(line, j, i, parts))
				return (0);
			if (!process_expansion(line, &i, msh, parts, quote))
				return (0);
			j = i;
		}
		else
			i++;
	}
	return (append_literal(line, j, i, parts));
}



// int	process_line(char *line, t_msh *msh, char ***parts)
// {
// 	int		i;
// 	int		j;
// 	char	quote;

// 	i = 0;
// 	j = 0;
// 	quote = 0;
// 	while (line[i])
// 	{
// 		if ((line[i] == '\'' || line[i] == '\"')
// 			&& (!quote || quote == line[i]))
// 			toggle_quote(&quote, line[i]);
// 		if (line[i] == '$' && quote != '\'')
// 		{
// 			if (!append_literal(line, j, i, parts))
// 				return (0);
// 			if (!process_expansion(line, &i, msh, parts))
// 				return (0);
// 			j = i;
// 		}
// 		else
// 			i++;
// 	}
// 	return (append_literal(line, j, i, parts));
// }
