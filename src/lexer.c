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

static int handle_quotes(char *line, int *i, char *result)
{
    char	quote;
    int		start;

    start = *i; // Remember the position of the opening quote
    quote = line[*i];
    (*i)++; // Move past the opening quote

    // Find the matching closing quote
    while (line[*i] && (line[*i] != quote))
        (*i)++;

    // Check for unclosed quote
    if (!line[*i])
    {
        // Use your error function (assuming p_syntax_error exists and works like ft_syntax_error)
        // You might need a different error message for unclosed quotes
        // For now, using a generic message:
        p_syntax_error(&quote); // Or pass a specific message string
        return (0); // Signal error
    }

    (*i)++; // Move past the closing quote

    // Append the entire segment (including start and end quotes) to result
    ft_strncat(result, &line[start], *i - start);

    return (1); // Signal success
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

int	lexer(char *line, char ***tokens) // Return int
{
    int		i;
    char	*result;
    int     success = 1; // Track success

    *tokens = NULL; // Initialize for safety
    // init_buff should ideally return int too
    init_buff(line, &result); // Assuming it handles malloc failure internally for now
    if (!result) return 0; // Check if init_buff failed

    i = 0;
    while (line[i] && success)
    {
        while (line[i] && (line[i] == 32 || (line[i] >= 9 && line[i] <= 13)))
        {
            push_token(tokens, result);
            i++;
        }
        if (!line[i]) break; // End of line check

        if (line[i] == 34 || line[i] == 39)
        {
            if (!handle_quotes(line, &i, result)) // Check return value
                success = 0; // Set flag on error
        }
        else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
        {
            if (!handle_operators(line, tokens, result, &i))
                success = 0; // Set flag on error
        }
        else
            handle_combined_token(line, &i, result);
    }
    // Push final token only if successful
    if (success)
        push_token(tokens, result);

    free(result);

    if (!success) {
        if (tokens && *tokens) {
            free_arr((void **)*tokens);
            *tokens = NULL;
        }
        return (0); // Return 0 on failure
    }
    return (1); // Return 1 on success
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
