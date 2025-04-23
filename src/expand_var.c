#include "includes/minishell.h"

static int safe_arr_push(char ***arr, const char *str)
{
    char *copy;

    copy = ft_strdup(str);
    if (!copy)
    {
        perror("ft_strdup failed in safe_arr_push");
        return (0); // Indicate failure
    }
    if (!arr_push(arr, copy))
    {
        perror("arr_push failed in safe_arr_push");
        free(copy); // Clean up the copy if arr_push failed
        return (0); // Indicate failure
    }
    free(copy); // arr_push made its own copy, free the intermediate one
    return (1); // Indicate success
}



char **expand_and_retokenize(char **tokens, t_vars *myenv)
{
	char **new_tokens;
	char *key;
	char *value;
	int i;

	new_tokens = NULL;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '$' && tokens[i][1] != '\0')
		{
			if (tokens[i][1] == '?')
			{
				// exit status logic wil come
			}
			else
			{
				key = ft_substr(tokens[i], 1, (ft_strlen(tokens[i])) - 1);
				if (!key) {
					perror("ft_substr failed for key");
					free_arr((void **)new_tokens);
					return (NULL);
				}
				value = get_var_value(myenv, key);
				free(key);
				if (!value)
					break ;
				// retokenize logic comes here
			}

		}
		else
			arr_push(&new_tokens, tokens[i]);
		i++;
	}
	return (new_tokens);
}
