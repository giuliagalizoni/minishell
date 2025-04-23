#include "includes/minishell.h"

static int safe_arr_push(char ***arr, const char *str)
{
	char *copy;

	copy = ft_strdup(str);
	if (!copy)
	{
		perror("ft_strdup failed in safe_arr_push");
		return (0);
	}
	if (!arr_push(arr, copy))
	{
		perror("arr_push failed in safe_arr_push");
		free(copy);
		return (0);
	}
	free(copy);
	return (1);
}

static int	handle_exit_stauts(t_msh *msh, char ***new_tokens)
{
	char *exit_status_str;
	int success;

	exit_status_str = ft_itoa(msh->exit_status);
	if (!exit_status_str) {
		perror("ft_itoa failed for exit status");
		return (0);
	}
	success = safe_arr_push(new_tokens, exit_status_str);
	free(exit_status_str);
	return (success);
}

static int	handle_regular_var(const char *token, t_msh *msh, char ***new_tokens)
{
	char *key;
	char *value;

	key = ft_substr(token, 1, ft_strlen(token) - 1);
	if (!key) {
		perror("ft_substr failed for key");
		return (0);
	}
	value = get_var_value(msh->myenv, key);
	free(key);
	if (value)
	{
		// Retokenize logic goes here
		if (!safe_arr_push(new_tokens, value))
			return (0);
	}
	return (1);
}

static int	handle_non_var(char *token, char ***new_tokens)
{
	if (!safe_arr_push(new_tokens, token))
		return (0);
	return (1);
}

char **expand_and_retokenize(char **tokens, t_msh *msh)
{
	char **new_tokens;
	int i;
	int success;

	new_tokens = NULL;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '$' && tokens[i][1] != '\0')
		{
			if (tokens[i][1] == '?')
				success = handle_exit_stauts(msh, &new_tokens);
			else
				success = handle_regular_var(tokens[i], msh, &new_tokens);
		}
		else
			success = handle_non_var(tokens[i], &new_tokens);
		i++;
	}
	if (!success)
	{
		free_arr((void **)new_tokens);
		return (NULL);
	}
	return (new_tokens);
}
