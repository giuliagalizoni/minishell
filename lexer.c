#include "includes/minishell.h"

char	**lexer(char *line)
{
	char	**tokens;
	char	*buff;
	char	quote;
	int		i;
	int		buffi;

	tokens = NULL;
	buff = (char *)malloc(ft_strlen(line) + 1); //if the buffer is big enough for the whole string, it will be big enough for the token
	if (!buff)
		return (NULL); // manage malloc errors later
	i = 0;
	buffi = 0;
	while(line[i])
	{
		while (line[i] && (line[i] == 32 || (line[i] >= 9 && line[i] <= 13))) // all the isspace() chars
			i++; //skip if there's space in the beggining
		if (!line[i])
			break ;
		if (line[i] == 39 || line[i] == 34) // '' || ""
		{
			if (buffi > 0) // closing quotes - end of token
			{
				buff[buffi] = '\0'; // null terminate buff
				arr_push(&tokens, buff); //push to tokens arr
				free(buff);
				buff = malloc(ft_strlen(line) + 1); //maybe I'll code an ft_realloc to make this cleaner
				buffi = 0;
			}
			quote = line[i++]; // store the code to know if its single or double
			while (line[i] && (line[i] != quote))
				buff[buffi++] = line[i++]; //copy the content inside quotes
			if (!line[i]) // open but never closed quotes
			{
				perror("Invalid quotes");
				return (free(buff), free_arr(tokens), NULL);
			}
			i++;
			buff[buffi] = '\0'; //here we're handling the cases where there's no quote
			arr_push(&tokens, buff);
			free(buff);
			buff = (char *)malloc(ft_strlen(line) + 1);
			buffi = 0;
		}
		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
		{
			if (buffi > 0)
			{
				buff[buffi] = '\0';
				arr_push(&tokens, buff);
				free(buff);
				buff = (char *)malloc(ft_strlen(line) + 1);
				buffi = 0;
			}
			buff[buffi++] = line[i];
			if ((line[i] == '<' && line[i + 1] == '<') || (line[i] == '>' && line[i + 1] == '>'))
				buff[buffi++] = line[++i];
			buff[buffi] = '\0';
			arr_push(&tokens, buff);
			free(buff);
			buff = (char *)malloc(ft_strlen(line) + 1);
			buffi = 0;
			i++;
		}
		else
		{
			while (line[i] && line[i] != ' ' && line[i] != '|' && line[i] != '<' && line[i] != '>' && line[i] != 34 && line[i] != 39)
				buff[buffi++] = line[i++];
			if (buffi > 0)
			{
				buff[buffi] = '\0';
				arr_push(&tokens, buff);
				free(buff);
				buff = (char *)malloc(ft_strlen(line) + 1);
				buffi = 0;
			}
		}
	}
	free(buff);
	return tokens;
}

int main() {
	char **tokens = lexer("     echo<<   \"Hello world\" |pipe");
	while (*tokens)
	{
		printf("token: %s\n", *tokens);
		tokens++;
	}
}
