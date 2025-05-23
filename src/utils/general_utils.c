#include "../includes/minishell.h"

int	is_equal(char *str1, char *str2)
{
	if (ft_strlen(str1) != ft_strlen(str2))
		return (0);
	if (!ft_strncmp(str1, str2, ft_strlen(str2)))
		return (1);
	return (0);
}

char	*ft_strncat(char *dest, const char *src, size_t n)
{
	size_t	i;
	size_t	dest_len;

	i = 0;
	dest_len = 0;
	while (dest[dest_len])
		dest_len++;
	while (i < n && src[i])
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] > s2[i])
			return (1);
		else if (s1[i] < s2[i])
			return (-1);
		i++;
	}
	return (0);
}

int	p_syntax_error(char *token)
{
	char	*prefix;
	char	*sufix;
	char	*actual_token;

	prefix = "minishell: syntax error near unexpected token `";
	sufix = "'\n";
	actual_token = token;
	if (!actual_token)
		actual_token = "newline";
	write(2, prefix, ft_strlen(prefix));
	write(2, actual_token, ft_strlen(actual_token));
	write(2, sufix, ft_strlen(sufix));
	return (0);
}

char	*ft_triplestrjoin(char *str1, char *str2, char *str3)
{
	char	*str1_2;
	char	*str1_2_3;

	if (!str1 || !str2 || !str3)
		return (NULL);
	str1_2 = ft_strjoin(str1, str2);
	if (str1_2 == NULL)
		return (NULL);
	str1_2_3 = ft_strjoin(str1_2, str3);
	if (str1_2_3 == NULL)
		return(NULL);
	free(str1_2);
	str1_2 = NULL;
	return (str1_2_3);
}
