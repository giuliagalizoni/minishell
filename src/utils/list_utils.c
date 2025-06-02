/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:02:33 by ggalizon          #+#    #+#             */
/*   Updated: 2025/06/02 13:02:37 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_outfile	*filelast(t_outfile *outfile)
{
	t_outfile	*tmp;

	tmp = outfile;
	if (tmp == NULL)
		return (NULL);
	while (tmp->next != NULL)
		tmp = tmp->next;
	return (tmp);
}

int	add_outfile(char **tokens, t_outfile **outfiles, int *i)
{
	t_outfile	*outfile;
	t_outfile	*tmp;

	outfile = malloc(sizeof(t_outfile));
	if (outfile == NULL)
		return (perror("malloc failed for outfile struct in add_outfile"), 0);
	if (is_equal(tokens[*i], ">"))
		outfile->is_append = 0;
	else if (is_equal(tokens[*i], ">>"))
		outfile->is_append = 1;
	if (!tokens[*i + 1])
		return (free(outfile), p_syntax_error(NULL), 0);
	outfile->filename = ft_strdup(tokens[*i + 1]);
	if (!outfile->filename)
		return (free(outfile), perror("ft_strdup failed add_outfile"), 0);
	outfile->next = NULL;
	(*i)++;
	if (!*outfiles)
	{
		*outfiles = outfile;
		return (1);
	}
	tmp = filelast(*outfiles);
	tmp->next = outfile;
	return (1);
}

static void	swap_vars(t_vars *a, t_vars *b)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = a->key;
	temp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = temp_key;
	b->value = temp_value;
}

void	sort_vars_list(t_vars *head)
{
	int		swapped;
	t_vars	*current;
	t_vars	*last_ptr;

	last_ptr = NULL;
	if (head == NULL || head->next == NULL)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = head;
		while (current->next != last_ptr)
		{
			if (ft_strcmp(current->key, current->next->key) > 0)
			{
				swap_vars(current, current->next);
				swapped = 1;
			}
			current = current->next;
		}
		last_ptr = current;
	}
}

t_vars	*find_var(t_vars *var, char *key)
{
	while (var)
	{
		if (is_equal(var->key, key))
			return (var);
		var = var->next;
	}
	return (NULL);
}
