#include "includes/minishell.h"

char	*prep_buffer(char *line, char *buff, char ***tokens, int *buffi)
{
		buff[(*buffi)] = '\0'; // null terminate buff
		arr_push(tokens, buff); //push to tokens arr
		free(buff);
		buff = malloc(ft_strlen(line) + 1); // allocate again
		if (!buff)
		{
			perror("malloc for buff failed");
			free_arr((void **)tokens);
			return (NULL);
		}
		*buffi = 0; // set the buffer index to 0 to start next token
		return (buff);
}

void	handle_quotes(char *line, char *buff, char ***tokens, int *i, int *buffi)
{
	char	quote;

	if (*buffi > 0) // closing quotes - end of token
		buff = prep_buffer(line, buff, tokens, buffi);
	quote = line[*i];
	(*i)++;
	while (line[*i] && (line[*i] != quote))
		buff[(*buffi)++] = line[(*i)++]; //copy the content inside quotes
	if (!line[*i]) // open but never closed quotes
	{
		perror("Invalid quotes");
		free(buff);
		free_arr((void**)tokens);
		return ;
	}
	(*i)++;
	buff = prep_buffer(line, buff, tokens, buffi);
}

void	handle_operator(char *line, char *buff, char ***tokens, int *i, int *buffi)
{
	if (*buffi > 0)
		buff = prep_buffer(line, buff, tokens, buffi);
	buff[(*buffi)++] = line[*i];
	if ((line[*i] == '<' && line[(*i) + 1] == '<') || (line[*i] == '>' && line[(*i) + 1] == '>')) // check for << and >>
		buff[(*buffi)++] = line[++(*i)];
	buff = prep_buffer(line, buff, tokens, buffi);
	(*i)++;
}

char *init_lexer(char *line, int *i, int *buffi)
{
	char	*buff;

	buff = (char *)malloc(ft_strlen(line) + 1); //if the buffer is big enough for the whole string, it will be big enough for the token
	if (!buff)
	{
		perror("malloc for buff failed");
		return (NULL);
	}
	*i = 0;
	*buffi = 0;
	return (buff);
}

char	**lexer(char *line, char ***tokens)
{
	char	*buff;
	int		i;
	int		buffi;

	buff = init_lexer(line, &i, &buffi);
	while(line[i])
	{
		while (line[i] && (line[i] == 32 || (line[i] >= 9 && line[i] <= 13))) // all the isspace() chars
			i++; //skip if there's space in the beggining
		if (!line[i])
			break ;
		if (line[i] == 39 || line[i] == 34) // create token inside quotes
			handle_quotes(line, buff, tokens, &i, &buffi);
		else if (line[i] == '|' || line[i] == '<' || line[i] == '>') // create tokens for operators
			handle_operator(line, buff, tokens, &i, &buffi);
		else // create all the other tokens
		{
			while (line[i] && line[i] != ' ' && line[i] != '|' && line[i] != '<'
				&& line[i] != '>' && line[i] != 34 && line[i] != 39) //all kinds of delimiters
				buff[buffi++] = line[i++];
			if (buffi > 0)
				buff = prep_buffer(line, buff, tokens, &buffi);
		}
	}
	return free(buff), *tokens; // review return value later
}

int main() {
	char **tokens = NULL;
	tokens = lexer(" < \"file echo<<\"   \"Hello world\" |pipe> 'output    '|another \"one more pipe     \"   >> cool", &tokens);
	while (*tokens)
	{
		printf("token: %s\n", *tokens);
		tokens++;
	}
}
