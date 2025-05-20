/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:05:24 by ggalizon          #+#    #+#             */
/*   Updated: 2025/05/20 16:05:01 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
