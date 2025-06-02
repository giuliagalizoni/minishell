/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:02:40 by ggalizon          #+#    #+#             */
/*   Updated: 2025/06/02 13:02:41 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_shell_operator(char *s)
{
	if (!s)
		return (0);
	if (ft_strcmp(s, ">") == 0 || ft_strcmp(s, "<") == 0
		|| ft_strcmp(s, ">>") == 0 || ft_strcmp(s, "<<") == 0
		|| ft_strcmp(s, "|") == 0)
		return (1);
	return (0);
}

int	p_syntax_error(char *token)
{
	char	*prefix;
	char	*sufix;
	char	*actual_token;

	prefix = "minishell: syntax error near unexpected token `";
	sufix = "'\n";
	actual_token = token;
	if (!actual_token)
		actual_token = "newline";
	write(2, prefix, ft_strlen(prefix));
	write(2, actual_token, ft_strlen(actual_token));
	write(2, sufix, ft_strlen(sufix));
	return (0);
}

static int	envp_len(t_vars *myenv)
{
	int		size;

	size = 0;
	while (myenv)
	{
		size++;
		myenv = myenv->next;
	}
	return (size);
}

char	**myenv_to_envp(t_vars *myenv)
{
	char	**envp;
	int		size;
	int		i;

	i = 0;
	size = envp_len(myenv);
	envp = (char **)malloc(sizeof(char *) * (size + 1));
	if (envp == NULL)
		return (NULL);
	while (myenv)
	{
		if (myenv->value)
			envp[i] = ft_triplestrjoin(myenv->key, "=", myenv->value);
		else
			envp[i] = ft_strdup(myenv->key);
		i++;
		myenv = myenv->next;
	}
	envp[i] = NULL;
	return (envp);
}

t_vars	*init_envp(char **envp)
{
	t_vars	*head;
	t_vars	*new_var;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		new_var = parse_var(envp[i]);
		if (!new_var)
		{
			clean_myenv(head);
			return (NULL);
		}
		push_list(&head, new_var);
		i++;
	}
	return (head);
}
