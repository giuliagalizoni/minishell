#include "includes/minishell.h"

//this could be later moved to another file
char *ft_strncat(char *dest, const char *src, size_t n)
{
	size_t i;
	size_t dest_len;

	i = 0;
	dest_len = 0;
	while (dest[dest_len])
		dest_len++;
	while (i < n && src[i])
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest);
}

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

static void	handle_quotes(char *line, int *i, char *result)
{
	char	quote;
	int		start;

	quote = line[*i];
	(*i)++;
	start = *i;
	while (line[*i] && (line[*i] != quote))
		(*i)++;
	if (!line[*i])
	{
		perror("Invalid quotes");
		return ;
	}
	ft_strncat(result, &line[start], *i - start);
	(*i)++;
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
	push_token(tokens, result);
	result[0] = line[*i];
	if ((line[*i] == '<' && line[*i + 1] == '<')
		|| (line[*i] == '>' && line[*i + 1] == '>'))
	{
		if (line[*i + 2] == '<' || line[*i + 2] == '>')
		{
			printf("minishell: syntax error near unexpected token `%c'\n", line[*i + 2]);
			return (0);
		}
		result[1] = line[(*i)++];
		result[2] = '\0';
	}
	else
		result[1] = '\0';
	push_token(tokens, result);
	(*i)++;
	return 1;
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

char	**lexer(char *line, char ***tokens)
{
	int		i;
	char	*result;

	init_buff(line, &result);
	i = 0;
	while (line[i])
	{
		while (line[i] && (line[i] == 32 || (line[i] >= 9 && line[i] <= 13)))
		{
			push_token(tokens, result);
			i++;
		}
		if (line[i] == 34 || line[i] == 39)
			handle_quotes(line, &i, result);
		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
		{
			if (!handle_operators(line, tokens, result, &i))
				return NULL;
		}

		else
			handle_combined_token(line, &i, result);
	}
	push_token(tokens, result);
	free(result);
	return (*tokens);
}
