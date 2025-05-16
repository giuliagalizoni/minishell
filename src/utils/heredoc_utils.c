#include "../includes/minishell.h"

static int	append_literal(char *line, int start, int end, char ***parts)
{
	char	*segment;

	if (end > start)
	{
		segment = ft_substr(line, start, end - start);
		if (!segment || !arr_push(parts, segment))
			return (free(segment), 0);
		free(segment);
	}
	return (1);
}

static void	toggle_quote(char *quote, char line_char)
{
	if (*quote == 0)
		*quote = line_char;
	else if (*quote == line_char)
		*quote = 0;
}

int	process_line(char *line, t_msh *msh, char ***parts)
{
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	quote = 0;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '\"') && (!quote || quote == line[i]))
			toggle_quote(&quote, line[i]);
		if (line[i] == '$' && quote != '\'')
		{
			if (!append_literal(line, j, i, parts)
				|| !process_expansion(line, &i, msh, parts))
				return (0);
			j = i;
		}
		else
			i++;
	}
	return (append_literal(line, j, i, parts));
}

static int	append_exit_status(t_msh *msh, char ***parts)
{
	char	*value;

	value = ft_itoa(msh->exit_status);
	if (!value || !arr_push(parts, value))
		return (free(value), 0);
	free(value);
	return (1);
}

static int	append_variable(char *line, int *i, t_msh *msh, char ***parts)
{
	int		start;
	char	*key;
	char	*value;

	start = *i + 1;
	(*i)++;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	key = ft_substr(line, start, *i - start);
	if (!key)
		return (0);
	value = get_var_value(msh->myenv, key);
	if (value)
	{
		if (!arr_push(parts, value))
			return (0);
	}
	return (1);
}

int	process_expansion(char *line, int *i, t_msh *msh, char ***parts)
{
	if (line[*i + 1] == '?')
	{
		if (!append_exit_status(msh, parts))
			return (0);
	}
	else if (line[*i + 1] && (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_'))
	{
		if (!append_variable(line, i, msh, parts))
			return (0);
	}
	else
	{
		if (!arr_push(parts, "$"))
			return (0);
		(*i)++;
	}
	return (1);
}
