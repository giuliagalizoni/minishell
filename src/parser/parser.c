/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 13:34:26 by ggalizon          #+#    #+#             */
/*   Updated: 2025/05/26 11:58:53 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_operators(t_command *command, char **tokens, int i)
{
	if (is_equal(tokens[i], "<<"))
	{
		command->is_heredoc = 1;
		command->heredoc_is_final = 1;
	}
	else if (is_equal(tokens[i], "<"))
		command->heredoc_is_final = 0;
	else if (i > 0 && is_equal(tokens[i - 1], "<"))
	{
		if (!arr_push(&command->input_redirect, tokens[i]))
			return (0);
	}
	else if (i > 0 && is_equal(tokens[i - 1], "<<"))
	{
		if (command->heredoc_delimiter)
			free(command->heredoc_delimiter);
		command->heredoc_delimiter = ft_strdup(tokens[i]);
		if (!command->heredoc_delimiter)
			return (return_error("ft_strdup failed for heredoc_delimiter"));
	}
	else if (!arr_push(&command->arguments, tokens[i]))
		return (0);
	return (1);
}

static int	set_name(t_command *command, char **tokens, t_vars *myenv)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (is_equal(tokens[i], "<") || is_equal(tokens[i], "<<") || \
			is_equal(tokens[i], ">") || is_equal(tokens[i], ">>"))
		{
			if (!tokens[i + 1])
				return (p_syntax_error(NULL));
			i += 2;
		}
		else
			break ;
	}
	if (tokens[i])
	{
		command->name = ft_strdup(tokens[i]);
		if (!command->name)
			return (perror("malloc failed for command name"), 0);
		if (!is_equal(tokens[i], ""))
			command->path = get_cmd_path(command->name, myenv);
	}
	return (1);
}

int	process_command(t_command *command, char **tokens, int *i, t_msh *msh)
{
	if (is_equal(tokens[*i], "|"))
	{
		if (!tokens[*i +1])
			return (0);
		command->is_pipe = 1;
		command->pipe_next = analyser(tokens + (*i) + 1,
				command->index + 1, msh);
		if (!command->pipe_next)
			return (clear_command_chain(command), 0);
		return (0);
	}
	else if (is_equal(tokens[*i], ">") || is_equal(tokens[*i], ">>"))
	{
		if (!add_outfile(command, tokens, &command->outfile, i))
			return (clear_command_chain(command), 0);
	}
	else
	{
		if (!check_operators(command, tokens, *i))
			return (clear_command_chain(command), 0);
	}
	return (1);
}

t_command	*analyser(char **tokens, int index, t_msh *msh)
{
	t_command	*command;
	int			i;

	if (!tokens || !tokens[0])
		return (NULL);
	command = malloc(sizeof(t_command));
	if (!command)
		return (perror("malloc failed for command struct"), NULL);
	command_init(command);
	command->index = index;
	if (!set_name(command, tokens, msh->myenv))
		return (free(command), NULL);
	i = 0;
	while (tokens[i])
	{ctrl-c when heredoc
ctrl-d when heredoc
		if (!process_command(command, tokens, &i, msh))
			break ;
		i++;
	}
	return (command);
}

int	parser(char *line, t_msh *msh)
{
	char	**tokens;
	char	**retokens;

	if (!line)
		return (1);
	tokens = NULL;
	if (!lexer(line, &tokens))
		return (1);
	if (!tokens)
		return (1);
	if (tokens && !check_invalid_syntax(tokens))
	{
		free_arr((void **)tokens);
		tokens = NULL;
		return (2);
	}
	retokens = expand_and_retokenize(tokens, msh);
	msh->command = analyser(retokens, 0, msh);
	if (!msh->command)
		return (2);
	free_arr((void **)tokens);
	free_arr((void **)retokens);
	return (0);
}
