#include "includes/minishell.h"

static void	push_token(char ***tokens, char *result)
{
	char	*token;

	if (result[0])
	{
		token = ft_strdup(result);
		arr_push(tokens, token);
		free(token);
		result[0] = '\0';
	}
}

static int	handle_quotes(char *line, int *i, char *result)
{
	char	quote;
	int		start;

	start = *i;
	quote = line[*i];
	(*i)++;
	while (line[*i] && (line[*i] != quote))
		(*i)++;
	if (!line[*i])
	{
		p_syntax_error(&quote);
		return (0);
	}
	(*i)++;
	ft_strncat(result, &line[start], *i - start);
	return (1);
}

static void	handle_combined_token(char *line, int *i, char *result)
{
	int	start;

	start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '<'
		&& line[*i] != '>' && line[*i] != 34 && line[*i] != 39)
		(*i)++;
	ft_strncat(result, &line[start], *i - start);
}

static int	handle_operators(char *line, char ***tokens, char *result, int *i)
{
	char	*err_str;

	push_token(tokens, result);
	result[0] = line[*i];
	if ((line[*i] == '<' && line[*i + 1] == '<')
		|| (line[*i] == '>' && line[*i + 1] == '>'))
	{
		if (line[*i + 2] == '<' || line[*i + 2] == '>')
		{
			err_str = ft_substr(line, *i + 2, 1);
			p_syntax_error(err_str);
			free(err_str);
			return (0);
		}
		result[1] = line[(*i)++];
		result[2] = '\0';
	}
	else
		result[1] = '\0';
	push_token(tokens, result);
	(*i)++;
	return (1);
}

static void	init_buff(char *line, char **result)
{
	*result = (char *)malloc((ft_strlen(line) + 1) * sizeof(char));
	if (!*result)
	{
		perror("malloc failed");
		return ;
	}
	*result[0] = '\0';
}

static int	process_token(char *line, int *i, char ***tokens, char *result)
{
	int	success;

	success = 1;
	while (line[*i] && (line[*i] == 32 || (line[*i] >= 9 && line[*i] <= 13)))
	{
		push_token(tokens, result);
		(*i)++;
	}
	if (!line[*i])
		return (success);
	if (line[*i] == 34 || line[*i] == 39)
	{
		if (!handle_quotes(line, i, result))
			success = 0;
	}
	else if (line[*i] == '|' || line[*i] == '<' || line[*i] == '>')
	{
		if (!handle_operators(line, tokens, result, i))
			success = 0;
	}
	else
		handle_combined_token(line, i, result);
	return (success);
}

int	lexer(char *line, char ***tokens)
{
	int		i;
	int		success;
	char	*result;

	init_buff(line, &result); //change this to return int
	if (!result)
		return (0);
	i = 0;
	success = 1;
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

// char	**lexer(char *line, char ***tokens)
// {
// 	int		i;
// 	char	*result;

// 	init_buff(line, &result);
// 	i = 0;
// 	while (line[i])
// 	{
// 		while (line[i] && (line[i] == 32 || (line[i] >= 9 && line[i] <= 13)))
// 		{
// 			push_token(tokens, result);
// 			i++;
// 		}
// 		if (line[i] == 34 || line[i] == 39)
// 			handle_quotes(line, &i, result);
// 		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
// 		{
// 			if (!handle_operators(line, tokens, result, &i))
// 				return (NULL);
// 		}
// 		else
// 			handle_combined_token(line, &i, result);
// 	}
// 	push_token(tokens, result);
// 	free(result);
// 	return (*tokens);
// }
