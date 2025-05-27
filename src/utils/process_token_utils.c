#include "../includes/minishell.h"

static int	extract_key_and_remainder(char *token, char **key, char **remainder)
{
	int	var_length;

	var_length = 1;
	while (token[var_length] && ft_isalnum(token[var_length]))
		var_length++;
	*key = ft_substr(token, 1, var_length - 1);
	if (!(*key))
		return (perror("failed creating key substring"), 0);
	*remainder = ft_strdup(token + var_length);
	if (!(*remainder))
	{
		free(*key);
		return (perror("failed duplicating remainder"), 0);
	}
	return (1);
}

static int	push_split_tokens(char *combined, char ***new_tokens)
{
	char	**split_value;
	int		success;
	int		i;

	split_value = ft_split(combined, ' ');
	if (!split_value)
		return (perror("failed splitting expanded token"), 0);
	success = 1;
	i = 0;
	while (split_value[i] && success)
	{
		success = safe_arr_push(new_tokens, split_value[i]);
		i++;
	}
	free_arr((void **)split_value);
	return (success);
}

static char	*expand_and_combine(char *key, char *remainder, t_msh *msh)
{
	char	*value;
	char	*combined;

	value = get_var_value(msh->myenv, key);
	free(key);
	if (value)
		combined = ft_strjoin(value, remainder);
	else
		combined = ft_strdup(remainder);
	free(remainder);
	return (combined);
}

int	handle_unquoted_var(char *token, t_msh *msh, char ***new_tokens)
{
	char	*key;
	char	*remainder;
	char	*combined;
	int		success;

	if (!extract_key_and_remainder(token, &key, &remainder))
		return (0);
	combined = expand_and_combine(key, remainder, msh);
	if (!combined)
		return (perror("failed combining expanded token"), 0);
	success = push_split_tokens(combined, new_tokens);
	free(combined);
	return (success);
}
