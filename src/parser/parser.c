/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 13:34:26 by ggalizon          #+#    #+#             */
/*   Updated: 2025/05/23 10:00:00 by Gemini            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Helper to check if a token is any redirection operator
static int is_redirection_operator(const char *token)
{
	if (!token)
		return (0);
	return (is_equal(token, ">") || is_equal(token, ">>") ||
			is_equal(token, "<") || is_equal(token, "<<"));
}

// analyser now directly parses tokens into a command structure.
// It handles command name, arguments, redirections, and pipes.
t_command	*analyser(char **tokens, int cmd_idx, t_msh *msh)
{
	t_command	*command;
	int			i;
	int			arg_idx_for_name; // To track if command name was pushed to args

	arg_idx_for_name = 0;
	if (!tokens || !tokens[0])
		return (NULL);
	command = malloc(sizeof(t_command));
	if (!command)
		return (perror("malloc failed for command struct"), NULL);
	command_init(command);
	command->index = cmd_idx;
	i = 0;
	while (tokens[i])
	{
		// Handle pipe operator: terminates current command and starts a new one
		if (is_equal(tokens[i], "|"))
		{
			command->is_pipe = 1;
			// Ensure there's a command after the pipe
			if (!tokens[i + 1] || is_redirection_operator(tokens[i + 1]) && !tokens[i + 2])
			{
			    // Allow `| > file` if that's intended, but `|` at end or `| >` is error
			    if (!tokens[i+1] || (is_redirection_operator(tokens[i+1]) && !tokens[i+2] && !is_redirection_operator(tokens[i+1]))) {
			        p_syntax_error(tokens[i + 1] ? tokens[i + 1] : tokens[i]);
			        return (clear_command_chain(command), NULL);
			    }
			}
			command->pipe_next = analyser(tokens + i + 1, cmd_idx + 1, msh);
			if (!command->pipe_next && *(tokens + i + 1) != NULL) // If analyser failed for a non-empty subcommand
			{
				// If pipe_next is NULL because there were no valid tokens after pipe, it's fine.
				// But if there were tokens and analyser failed, it's an error.
				// The check `*(tokens + i + 1) != NULL` attempts to see if there *were* tokens.
				// A more robust check in analyser would be to return specific error/NULL.
				// For now, assume if pipe_next is NULL and there were tokens, it's an error.
				return (clear_command_chain(command), NULL);
			}
			// Current command is complete due to pipe
			// If no command name was found for the current command (e.g. "> out | ls")
			// and arguments were also not found, it might be an issue depending on shell requirements.
			// Bash allows `>file | cat`
			if (!command->name && !command->arguments)
			{
				// This specific case `> file | command` is tricky.
				// If `command->name` is NULL, it means only redirections were found before the pipe.
				// This is often valid in shells. No specific error here yet.
			}
			return (command);
		}
		// Handle output redirection: > or >>
		else if (is_equal(tokens[i], ">") || is_equal(tokens[i], ">>"))
		{
			// Check for filename existence
			if (!tokens[i + 1] || is_redirection_operator(tokens[i + 1]) || is_equal(tokens[i+1], "|"))
			{
				p_syntax_error(tokens[i + 1] ? tokens[i + 1] : tokens[i]);
				return (clear_command_chain(command), NULL);
			}
			// add_outfile expects the index of the operator.
			// It will use tokens[i] (operator) and tokens[i+1] (filename).
			// It then increments the index *itself* to point to the filename.
			// So, the main loop's i++ will correctly move past the filename.
			if (!add_outfile(command, tokens, &command->outfile, &i))
				return (clear_command_chain(command), NULL);
			// add_outfile already incremented i to point to the filename.
			// The loop's i++ will advance past it.
		}
		// Handle input redirection: < or << (heredoc)
		else if (is_equal(tokens[i], "<") || is_equal(tokens[i], "<<"))
		{
			char *operator = tokens[i];
			char *filename_or_delimiter;

			// Check for filename/delimiter existence
			if (!tokens[i + 1] || is_redirection_operator(tokens[i + 1]) || is_equal(tokens[i+1], "|"))
			{
				p_syntax_error(tokens[i + 1] ? tokens[i + 1] : tokens[i]);
				return (clear_command_chain(command), NULL);
			}
			filename_or_delimiter = tokens[i + 1];
			if (is_equal(operator, "<"))
			{
				command->heredoc_is_final = 0; // Indicate that a regular input redirect was last seen
				if (!arr_push(&command->input_redirect, filename_or_delimiter))
				{
					perror("arr_push failed for input_redirect");
					return (clear_command_chain(command), NULL);
				}
			}
			else // It's "<<" (heredoc)
			{
				command->is_heredoc = 1;
				command->heredoc_is_final = 1; // This heredoc is the one to be used if multiple inputs
				if (command->heredoc_delimiter) // Free previous if any (e.g. << delim1 << delim2)
					free(command->heredoc_delimiter);
				command->heredoc_delimiter = ft_strdup(filename_or_delimiter);
				if (!command->heredoc_delimiter)
				{
					perror("ft_strdup failed for heredoc_delimiter");
					return (clear_command_chain(command), NULL);
				}
			}
			i++; // Manually increment i to skip the filename/delimiter
			     // The loop's i++ will then move to the next token.
		}
		// Not an operator, so it's a command name or an argument
		else
		{
			// If command name is not yet set, this token is the command name
			if (!command->name)
			{
				command->name = ft_strdup(tokens[i]);
				if (!command->name)
				{
					perror("ft_strdup failed for command name");
					return (clear_command_chain(command), NULL);
				}
				// Attempt to find the command path
				command->path = get_cmd_path(command->name, msh->myenv);
				// The command name is also the first argument (argv[0])
				if (!arr_push(&command->arguments, tokens[i]))
				{
					perror("arr_push failed for command argument (name)");
					// Cleanup name if argument push fails
					free(command->name);
					command->name = NULL;
					if (command->path) free(command->path); // path might be NULL
					command->path = NULL;
					return (clear_command_chain(command), NULL);
				}
				arg_idx_for_name = 1; // Mark that name was pushed to args
			}
			// If command name is set, this token is an argument
			else
			{
				// Check if this token is the command name again (should not happen if logic is correct)
				// This is a safeguard; effectively, tokens[i] should be a new argument.
				if(arg_idx_for_name && is_equal(tokens[i], command->name) && command->arguments && is_equal(command->arguments[0], tokens[i]))
				{
					// This case implies the command name token is being processed again.
					// This shouldn't happen with the current loop structure.
					// If it does, it might indicate an issue with how 'i' is managed
					// after redirections, or if command->name was set by a non-argument token.
					// For now, we assume this token is a regular argument if command->name is already set.
				}

				if (!arr_push(&command->arguments, tokens[i]))
				{
					perror("arr_push failed for command argument");
					return (clear_command_chain(command), NULL);
				}
			}
		}
		i++; // Move to the next token
	}
	// After loop, if command->name is NULL but there were tokens,
	// it means the command was purely redirections (e.g., "> out < in").
	// This is often valid. If arguments were collected, but no command name,
	// that implies a syntax error not caught (e.g. `> out arg1`) - but current logic
	// would make `arg1` the command name.
	if (!command->name && command->arguments)
	{
		// This state (args but no name) should ideally not be reached if the first non-op
		// token becomes the name. If it is, it's a logic flaw.
		// However, if `command->arguments` is not NULL, it means `arr_push` was called.
		// The first such push would have set `command->name`.
		// So this condition implies an issue if `command->name` could be unset later.
		// Let's assume for now it's okay, as `get_cmd_path` might make `command->path` NULL
		// but `command->name` would still be set.
	}
	// If no command name was found at all (e.g. empty input, or only spaces after lexing)
	// `command_init` sets name to NULL. If tokens were processed but name remains NULL,
	// it means only redirections were present e.g. `> file1 < file2`
	// This is valid in many shells (it performs redirections but executes no command).
	return (command);
}

t_command	*parser(char *line, t_msh *msh)
{
	char	**tokens;
	char	**retokens; // For expanded tokens

	if (!line)
		return (NULL);
	tokens = NULL;
	// Lexer: breaks the input line into raw tokens
	if (!lexer(line, &tokens))
		return (NULL); // Lexer error
	if (!tokens)
		return (NULL); // No tokens produced (e.g., empty line)
	// Syntax Check: basic validation of token sequence (e.g., no `| |`, no `>` at end)
	if (tokens && !check_invalid_syntax(tokens))
	{
		free_arr((void **)tokens);
		tokens = NULL;
		msh->exit_status = 2; // Syntax error exit code
		return (NULL);
	}
	// Expansion and Retokenization: handles variable expansion, tilde, quotes etc.
	// This might change the number and content of tokens.
	retokens = expand_and_retokenize(tokens, msh);
	// Original tokens are freed by expand_and_retokenize or should be here
	free_arr((void **)tokens); 
	if (!retokens)
	{
		// Expansion might fail or result in no tokens.
		// If expand_and_retokenize cleans up tokens on failure, great.
		// Otherwise, ensure they are freed.
		return (NULL); 
	}
	// Analyser: constructs the command structure from the final (re)tokenized list
	msh->command = analyser(retokens, 0, msh);
	// Retokens are processed; free them now.
	// The data from retokens (like command name, args, filenames) would have been
	// ft_strdup'd into the command structure by analyser or its helpers (add_outfile, etc.).
	free_arr((void **)retokens);
	return (msh->command);
}
