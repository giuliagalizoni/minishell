#include "includes/minishell.h"

static int	is_redirection(char *token)
{
	if (!token)
		return (0);
	return (is_equal(token, ">") || is_equal(token, "<")
		|| is_equal(token, ">>") || is_equal(token, "<<"));
}

static int	is_pipe(char *token)
{
	if (!token)
		return (0);
	return (is_equal(token, "|"));
}

static int	is_operator(char *token)
{
	if (!token)
		return (0);
	return (is_pipe(token) || is_redirection(token));
}

static int	tokens_check(char **tokens, int i)
{
	if (is_redirection(tokens[i]))
	{
		if (!tokens[i + 1] || is_operator(tokens[i + 1]))
			return (p_syntax_error(tokens[i + 1]));
	}
	else if (is_pipe(tokens[i]))
	{
		if (!tokens[i + 1] || is_pipe(tokens[i + 1]))
			return (p_syntax_error(tokens[i + 1]));
	}
	return (1);
}

	//syntax error code 2
int	check_invalid_syntax(char **tokens)
{
	int	i;
	int	last;

	if (!tokens || !tokens[0])
		return (1);
	if (is_operator(tokens[0]) && is_pipe(tokens[0]))
		return (p_syntax_error(tokens[0]));
	last = -1;
	while (tokens[last + 1])
		last++;
	if (last >= 0 && is_operator(tokens[last]))
		return (p_syntax_error(NULL));
	i = 0;
	while (tokens[i])
	{
		if (!tokens_check(tokens, i))
			return (0);
		i++;
	}
	return (1);
}
