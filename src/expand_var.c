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

static int	handle_exit_stauts(t_msh *msh, char ***new_tokens)
{
	char *exit_status_str;
	int success;

	exit_status_str = ft_itoa(msh->exit_status);
	if (!exit_status_str) {
		perror("ft_itoa failed for exit status");
		free_arr((void **)new_tokens);
		return (0);
	}
	success = safe_arr_push(&new_tokens, exit_status_str);
	free(exit_status_str);
	return (success);
}

char **expand_and_retokenize(char **tokens, t_msh *msh)
{
	char **new_tokens;
	char *key;
	char *value;
	int i;
	int success;

	new_tokens = NULL;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '$' && tokens[i][1] != '\0')
		{
			if (tokens[i][1] == '?')
				handle_exit_stauts(msh, new_tokens);
			else
			{
				key = ft_substr(tokens[i], 1, (ft_strlen(tokens[i])) - 1);
				if (!key) {
					perror("ft_substr failed for key");
					free_arr((void **)new_tokens);
					return (NULL);
				}
				value = get_var_value(msh->myenv, key);
				free(key);
				if (value)
				{
					// retokenize logic comes here
					if (!safe_arr_push(&new_tokens, value))
					{
						perror("arr_push failed for variable value");
						free_arr((void **)new_tokens);
						return (NULL);
					}
				}
			}
		}
		else
		{
			if (!safe_arr_push)
			{
				perror("arr_push failed for token copy");
				free_arr((void **)new_tokens);
				return (NULL);
			}
		}
		i++;
	}
	return (new_tokens);
}
