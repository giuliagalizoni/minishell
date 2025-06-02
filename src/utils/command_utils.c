/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:01:35 by ggalizon          #+#    #+#             */
/*   Updated: 2025/06/02 13:01:36 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	command_init(t_command *command)
{
	command->name = NULL;
	command->path = NULL;
	command->arguments = NULL;
	command->input_redirect = NULL;
	command->is_heredoc = 0;
	command->heredoc_delimiter = NULL;
	command->heredoc_fd[0] = -1;
	command->heredoc_fd[1] = -1;
	command->heredoc_is_final = 0;
	command->outfile = NULL;
	command->is_pipe = 0;
	command->pipe_next = NULL;
	command->index = 0;
}

static void	clear_outfile_list(t_outfile *head)
{
	t_outfile	*current;
	t_outfile	*next_node;

	current = head;
	while (current)
	{
		next_node = current->next;
		if (current->filename)
			free(current->filename);
		free(current);
		current = next_node;
	}
}

static void	recursive_command_clear(t_command *command)
{
	clear_command_chain(command->pipe_next);
	command->pipe_next = NULL;
}

void	clear_command_chain(t_command *command)
{
	if (!command)
		return ;
	if (command->pipe_next)
		recursive_command_clear(command);
	if (command->name)
		free(command->name);
	if (command->path)
		free(command->path);
	if (command->arguments)
		free_arr((void **)command->arguments);
	if (command->input_redirect)
		free_arr((void **)command->input_redirect);
	if (command->heredoc_delimiter)
		free(command->heredoc_delimiter);
	if (command->heredoc_fd[0] != -1)
		close(command->heredoc_fd[0]);
	if (command->heredoc_fd[1] != -1)
		close(command->heredoc_fd[1]);
	if (command->outfile)
	{
		clear_outfile_list(command->outfile);
		command->outfile = NULL;
	}
	free(command);
}

int	count_commands(t_command *command)
{
	int	count;

	count = 0;
	while (command)
	{
		count++;
		command = command->pipe_next;
	}
	return (count);
}
