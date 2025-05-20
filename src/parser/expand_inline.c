/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_inline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:03:10 by ggalizon          #+#    #+#             */
/*   Updated: 2025/05/20 15:03:11 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	toggle_quote(char *quote, char line_char)
{
	if (*quote == 0)
		*quote = line_char;
	else if (*quote == line_char)
		*quote = 0;
}

static char	*join_strings(char **arr)
{
	char	*result;
	size_t	total_len;
	int		i;

	if (!arr || !arr[0])
		return (ft_strdup(""));
	total_len = 0;
	i = 0;
	while (arr[i])
	{
		total_len += ft_strlen(arr[i]);
		i++;
	}
	result = (char *)malloc(sizeof(char) * (total_len + 1));
	if (!result)
		return (NULL);
	result[0] = '\0';
	i = 0;
	while (arr[i])
	{
		ft_strlcat(result, arr[i], total_len + 1);
		i++;
	}
	return (result);
}

char	*expand_inline(char *line, t_msh *msh)
{
	char	**parts;
	char	*expanded;

	parts = NULL;
	if (!process_line(line, msh, &parts))
		return (free((void **)parts), ft_strdup(line));
	expanded = join_strings(parts);
	free_arr((void **)parts);
	if (!expanded)
		return (ft_strdup(""));
	return (expanded);
}
