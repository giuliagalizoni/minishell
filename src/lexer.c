#include "includes/minishell.h"

static void	push_reset(char ***tokens, char *line, int *start, int *len)
{
	char	*token;

	token = ft_substr(line, *start, *len);
	arr_push(tokens, token);
	free(token);
	*start = 0;
	*len = 0;
}
static void	handle_quotes(char *line, char ***tokens, int *i, int *start, int *len)
{
	char quote;

	quote = line[*i];
	(*i)++;
	*start = *i;
	while (line[*i] && (line[*i] != quote))
		(*i)++;
	if (!line[*i])
	{
		perror("Invalid quotes");
		free_arr((void**)tokens);
		return ;
	}
	*len = *i - *start;
	push_reset(tokens, line, start, len);
	(*i)++;
}

static void	handle_operators(char *line, char ***tokens, int *i, int *start, int *len)
{
	*start = *i;
	if ((line[*i] == '<' && line[(*i) + 1] == '<') || (line[*i] == '>' && line[(*i) + 1] == '>'))
		(*i)++;
	*len = (*i - *start) + 1;
	push_reset(tokens, line, start, len);
	(*i)++;
}

static void	handle_tokens(char *line, char ***tokens, int *i, int *start, int *len)
{
	*start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '|' && line[*i] != '<'
		&& line[*i] != '>' && line[*i] != 34 && line[*i] != 39)
		(*i)++;
	*len = *i - *start;
	push_reset(tokens, line, start, len);
	if (line[*i])
		(*i)++;
}
char **lexer(char *line, char ***tokens)
{
	int i;
	int start;
	int len;

	i = 0;
	start = 0;
	len = 0;
	while(line[i])
	{
		while (line[i] && (line[i] == 32 || (line[i] >= 9 && line[i] <= 13)))
			i++;
		if (!line[i])
			break ;
		if (line[i] == 39 || line[i] == 34)
			handle_quotes(line, tokens, &i, &start, &len);
		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			handle_operators(line, tokens, &i, &start, &len);
		else
			handle_tokens(line, tokens, &i, &start, &len);
	}
	return *tokens;
}
