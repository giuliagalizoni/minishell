#include "../includes/minishell.h"

void	free_arr(void **arr)
{
	size_t	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
}

char	**arr_push(char ***arr, char *str)
{
	char	**new_arr;
	int		len;
	int		i;

	len = 0;
	while (*arr && (*arr)[len])
		len++;
	new_arr = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_arr[i] = (*arr)[i];
		i++;
	}
	new_arr[i] = ft_strdup(str);
	if (!new_arr[i])
		return (free_arr((void **)new_arr), new_arr = NULL, NULL);
	new_arr[i + 1] = NULL;
	if (*arr)
		free(*arr);
	*arr = new_arr;
	return (new_arr);
}
