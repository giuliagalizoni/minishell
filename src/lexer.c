#include "includes/minishell.h"

// static void	push_reset(char ***tokens, char *line, int *start, int *len)
// {
// 	char	*token;

// 	token = ft_substr(line, *start, *len);
// 	arr_push(tokens, token);
// 	free(token);
// 	*start = 0;
// 	*len = 0;
// }

//this could be later moved to another file
char *ft_strncat(char *dest, const char *src, size_t n)
{
	size_t i = 0;
	size_t dest_len = 0;

	while (dest[dest_len])
		dest_len++;
	while (i < n && src[i])
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return dest;
}

static void	push_token(char ***tokens, char *result)
{
	char *token;
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
	int	start;

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
	ft_strncat(result, &line[start], *i-start);
	(*i)++;
}

// static void	handle_operators(char *line, char ***tokens, int *i, int *start, int *len)
// {
// 	*start = *i;
// 	if ((line[*i] == '<' && line[(*i) + 1] == '<') || (line[*i] == '>' && line[(*i) + 1] == '>'))
// 		(*i)++;
// 	*len = (*i - *start) + 1;
// 	push_reset(tokens, line, start, len);
// 	(*i)++;
// }

// static void	handle_tokens(char *line, char ***tokens, int *i, int *start, int *len)
// {
// 	*start = *i;
// 	while (line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '<'
// 		&& line[*i] != '>' && line[*i] != 34 && line[*i] != 39)
// 		(*i)++;
// 	*len = *i - *start;
// 	push_reset(tokens, line, start, len);
// }

static void	handle_combined_token(char *line, int *i, char *result)
{
	int	start;

	start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '<'
		&& line[*i] != '>' && line[*i] != 34 && line[*i] != 39)
		(*i)++;
	ft_strncat(result, &line[start], *i-start);
}

char	**lexer(char *line, char ***tokens)
{
	int i;
	char *result;

	result = (char *)malloc((ft_strlen(line) + 1) * sizeof(char));
	if (!result)
	{
		perror("malloc failed");
		return NULL;
	}
	result[0] = '\0';
	i = 0;
	while(line[i])
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
			push_token(tokens, result);
			result[0] = line[i];
			if ((line[i] == '<' && line[i + 1] == '<') || (line[i] == '>' && line[i + 1] == '>'))
			{
				result[1] = line[i++];
				result[2] = '\0';
			}
			push_token(tokens, result);
			i++;
		}
		else
			handle_combined_token(line, &i, result);
	}
	push_token(tokens, result);
	free(result);
	return *tokens;
}
// char **lexer(char *line, char ***tokens)
// {
// 	int i;
// 	int start;
// 	int len;

// 	i = 0;
// 	start = 0;
// 	len = 0;
// 	while(line[i])
// 	{
// 		while (line[i] && (line[i] == 32 || (line[i] >= 9 && line[i] <= 13)))
// 			i++;
// 		if (!line[i])
// 			break ;
// 		if (line[i] == 39 || line[i] == 34)
// 			handle_quotes(line, tokens, &i, &start, &len);
// 		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
// 			handle_operators(line, tokens, &i, &start, &len);
// 		else
// 			handle_tokens(line, tokens, &i, &start, &len);
// 	}
// 	return *tokens;
// }
