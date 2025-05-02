#include "includes/minishell.h"

static int	init_buff(char *line, char **result)
{
	*result = (char *)malloc((ft_strlen(line) + 1) * sizeof(char));
	if (!*result)
	{
		perror("malloc failed");
		return (0);
	}
	*result[0] = '\0';
	return (1);
}

int	lexer(char *line, char ***tokens)
{
	int		i;
	int		success;
	char	*result;

	success = init_buff(line, &result);
	if (!success)
		return (0);
	i = 0;
	while (line[i] && success)
		success = process_token(line, &i, tokens, result);
	if (success)
		push_token(tokens, result);
	free(result);
	if (!success)
	{
		if (tokens && *tokens)
		{
			free_arr((void **)*tokens);
			*tokens = NULL;
		}
		return (0);
	}
	return (1);
}
