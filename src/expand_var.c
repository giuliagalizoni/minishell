#include "includes/minishell.h"

static int	safe_arr_push(char ***arr, const char *str)
{
	char	*copy;

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

static int	handle_exit_status(t_msh *msh, char ***new_tokens)
{
	char	*exit_status_str;
	int		success;
	(void)msh;

	exit_status_str = ft_itoa(g_exit_status);
	if (!exit_status_str)
	{
		perror("ft_itoa failed for exit status");
		return (0);
	}
	success = safe_arr_push(new_tokens, exit_status_str);
	free(exit_status_str);
	return (success);
}

static int	retokenize(char ***new_tokens, char *value)
{
	char	**temp_tokens;
	int		i;
	int		success;

	temp_tokens = ft_split(value, ' ');
	if (!temp_tokens)
	{
		perror("ft_split failed or returned no words");
		return (0);
	}
	i = 0;
	success = 1;
	while (temp_tokens[i] && success)
	{
		success = safe_arr_push(new_tokens, temp_tokens[i]);
		i++;
	}
	free_arr((void **)temp_tokens);
	if (!success)
		return (0);
	return (1);
}

// static int	handle_regular_var(char *token, t_msh *msh, char ***new_tokens)
// {
// 	char	*key;
// 	char	*value;
// 	int		success;

// 	success = 1;
// 	key = ft_substr(token, 1, ft_strlen(token) - 1);
// 	if (!key)
// 	{
// 		perror("ft_substr failed for key");
// 		return (0);
// 	}
// 	value = get_var_value(msh->myenv, key);
// 	free(key);
// 	if (value)
// 		success = retokenize(new_tokens, value);
// 	return (success);
// }

// static int	handle_non_var(char *token, char ***new_tokens)
// {
// 	if (!safe_arr_push(new_tokens, token))
// 		return (0);
// 	return (1);
// }


static int	handle_double_quoted(char *token, t_msh *msh, char ***new_tokens)
{
    char	*inner_content;
    char	*key;
    char	*value;
    char	*exit_status_str;
    int		success = 1;
    size_t  len;

    len = ft_strlen(token);
    if (len < 2) { // Should not happen if lexer is correct, but safety check
        return safe_arr_push(new_tokens, token); // Push as is? Or error?
    }

    // Extract content without the surrounding double quotes
    inner_content = ft_substr(token, 1, len - 2);
    if (!inner_content) {
        perror("ft_substr failed removing quotes");
        return (0);
    }

    // --- Expansion Logic (Simplified: only handles "$VAR" or "$?") ---
    // A full implementation needs to iterate through inner_content
    // finding '$' and expanding, while copying literal parts.
    // This simplified version only works if inner_content is exactly $VAR or $?

    if (inner_content[0] == '$' && inner_content[1] != '\0')
    {
        if (inner_content[1] == '?') // Handle "$?"
        {
            exit_status_str = ft_itoa(g_exit_status); // Or msh->exit_status
            if (!exit_status_str) {
                perror("ft_itoa failed"); success = 0;
            } else {
                success = safe_arr_push(new_tokens, exit_status_str); // Push single token
                free(exit_status_str);
            }
        }
        else // Handle "$VAR"
        {
            key = ft_substr(inner_content, 1, ft_strlen(inner_content) - 1);
            if (!key) {
                perror("ft_substr failed"); success = 0;
            } else {
                value = get_var_value(msh->myenv, key);
                free(key);
                if (value) {
                    success = safe_arr_push(new_tokens, value); // Push single token
                }
                // else: var not found, push nothing (empty string equivalent)
            }
        }
    }
    else // Content inside "..." is literal (or needs more complex expansion)
    {
        success = safe_arr_push(new_tokens, inner_content); // Push literal content
    }
    // --- End Simplified Expansion Logic ---

    free(inner_content); // Free the substring
    return (success);
}

static int	handle_unquoted_var(char *token, t_msh *msh, char ***new_tokens)
{
    char	*key;
    char	*value;
    int		success;

    success = 1;
    key = ft_substr(token, 1, ft_strlen(token) - 1); // Key is token without '$'
    if (!key) {
        perror("ft_substr failed for key");
        return (0);
    }
    value = get_var_value(msh->myenv, key);
    free(key);
    if (value)
        success = retokenize(new_tokens, value); // Use retokenize (ft_split)
    return (success);
}

static int	handle_literal(char *token, char ***new_tokens)
{
    return (safe_arr_push(new_tokens, token));
}

char	**expand_and_retokenize(char **tokens, t_msh *msh)
{
    char	**new_tokens;
    int		i;
    int		success;
    size_t  len;

    new_tokens = NULL;
    i = 0;
    success = 1;
    if (!tokens) return NULL;

    while (tokens[i] && success)
    {
        len = ft_strlen(tokens[i]);
        // Check for single quotes: '...'
        if (len >= 2 && tokens[i][0] == '\'' && tokens[i][len - 1] == '\'')
        {
            // Remove quotes, push literal content as single token
            char *literal_content = ft_substr(tokens[i], 1, len - 2);
            if (!literal_content) {
                perror("ft_substr failed removing single quotes");
                success = 0; // Malloc failure
            } else {
                success = safe_arr_push(&new_tokens, literal_content);
                free(literal_content);
            }
        }
        // Check for double quotes: "..."
        else if (len >= 2 && tokens[i][0] == '"' && tokens[i][len - 1] == '"')
        {
            // Handle expansion within double quotes (no word splitting)
            success = handle_double_quoted(tokens[i], msh, &new_tokens);
        }
        // Check for unquoted variable: $...
        else if (tokens[i][0] == '$' && tokens[i][1] != '\0')
        {
            if (tokens[i][1] == '?') // Unquoted $?
                success = handle_exit_status(msh, &new_tokens); // Pushes single token
            else // Unquoted $VAR
                success = handle_unquoted_var(tokens[i], msh, &new_tokens); // Performs word splitting
        }
        // Otherwise, it's a literal token
        else
        {
            success = handle_literal(tokens[i], &new_tokens);
        }

        if (!success) break; // Exit loop on error
        i++;
    }

    // Cleanup on failure
    if (!success)
    {
        free_arr((void **)new_tokens);
        return (NULL);
    }

    return (new_tokens);
}

// char	**expand_and_retokenize(char **tokens, t_msh *msh)
// {
// 	char	**new_tokens;
// 	int		i;
// 	int		success;

// 	new_tokens = NULL;
// 	i = 0;
// 	// TODO i get a segfault here if the line input to the shell is empty
// 	// I imagine that has to be caught further upstream tho
// 	while (tokens[i])
// 	{
// 		if (tokens[i][0] == '$' && tokens[i][1] != '\0')
// 		{
// 			if (tokens[i][1] == '?')
// 				success = handle_exit_status(msh, &new_tokens);
// 			else
// 				success = handle_regular_var(tokens[i], msh, &new_tokens);
// 		}
// 		else
// 			success = handle_non_var(tokens[i], &new_tokens);
// 		i++;
// 	}
// 	if (!success)
// 	{
// 		free_arr((void **)new_tokens);
// 		return (NULL);
// 	}
// 	return (new_tokens);
// }
