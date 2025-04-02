#include "includes/minishell.h"

char	**lexer(char *line)
{
	char	**tokens;
	int		inside_quotes; // 0 for no, 1 for ' and 2 for "
	int		buff_size;
	char	*buff;
	int		i;

	buff_size = ft_strlen(line);
	buff = (char *)malloc(buff_size); //if the buffer is big enough for the whole string, it will be big enough for the token
	inside_quotes = 0;
	tokens = NULL;
	i = 0;
	while(line[i])
	{
		while (line[i] && (line[i] == 32 || (line[i] >= 9 && line[i] <= 13))) // all the isspace() chars
			i++; //skip if there's space in the beggining
		if (line[i] == 39) // ''
		{
			if (inside_quotes == 1) // toggle the flag
				inside_quotes = 0;
			else
				inside_quotes = 1;
			i++; // skip the quotes
		}
		if (line[i] == 34) // ""
		{
			if (inside_quotes == 2)
				inside_quotes = 0;
			else
				inside_quotes = 2;
		}
		if (!inside_quotes)
		{
			if (line[i] == ' ' || line[i] == '<' || line[i] == '>' || line[i] == '|' || line[i] == '\0') //delimitors or end of string
			{
				buff[i] = '\0'; //null terminate the buffer
				arr_push(&tokens, buff); //push to array of tokens
				free(buff); // free pointer
				i++; //is this necessary?
			}
		}
		buff[i] = line[i];
		i++;
	}
	return tokens;
}
