#include "../includes/minishell.h"

static int	is_n_flag(char *arg)
{
	int i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while(arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

char	*trim_quotes(char *str)
{
    size_t	len;
    char	*inner_content;

    if (!str)
        return (NULL);

    len = ft_strlen(str);

    // Check if the string is enclosed in matching quotes (single or double)
    if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') || \
                     (str[0] == '\'' && str[len - 1] == '\'')))
    {
        // Extract the content inside the quotes
        inner_content = ft_substr(str, 1, len - 2);
        if (!inner_content)
            return (ft_strdup(str)); // Fallback on substr error, return copy of original

        // If the content INSIDE the quotes IS a shell operator
        if (is_shell_operator(inner_content))
        {
            // Return the inner content (the operator itself, without the quotes)
            // inner_content is already a new string from ft_substr.
            return (inner_content);
        }
        else
        {
            // Content inside quotes is NOT an operator (e.g., "abc").
            // For this specific trim_quotes logic, we return the original string with quotes.
            free(inner_content); // Not using this path for inner_content
            return (ft_strdup(str));
        }
    }

    // Not enclosed in quotes, or the special condition wasn't met
    // Return a copy of the original string.
    return (ft_strdup(str));
}

// char	*trim_quotes(char *str)
// {
// 	size_t	len;
// 	char	*trimmed;

// 	if (!str)
// 		return (NULL);
// 	len = ft_strlen(str);
// 	if ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len - 1] == '\''))
// 	{
// 		trimmed = ft_substr(str, 1, len - 2);
// 		return (trimmed);
// 	}
// 	return (ft_strdup(str));
// }

int	echo(t_command *cmd)
{
	int	i;
	int	flag;
	char *arg;

	flag = 1;
	if (!cmd->arguments[1])
	{
		printf("\n");
		return (0);
	}
	i = 1;
	while (cmd->arguments[i] && is_n_flag(cmd->arguments[i]))
	{
		flag = 0;
		i++;
	}
	while(cmd->arguments[i])
	{
		arg = trim_quotes(cmd->arguments[i]);
		printf("%s", arg);
		free(arg);
		if (cmd->arguments[i + 1])
			printf(" ");
		i++;
	}
	if (flag)
		printf("\n");
	return (0);
}
