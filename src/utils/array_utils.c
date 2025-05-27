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
		return (perror("malloc failed in arr_push for new_arr"), NULL);
	i = 0;
	while (i < len)
	{
		new_arr[i] = (*arr)[i];
		i++;
	}
	new_arr[i] = ft_strdup(str);
	if (!new_arr[i])
		return (free_arr((void **)new_arr),
			perror("ft_strdup failed in arr_push"), new_arr = NULL, NULL);
	new_arr[i + 1] = NULL;
	if (*arr)
		free(*arr);
	*arr = new_arr;
	return (new_arr);
}

int	array_length(void **arr)
{
	int	count;

	count = 0;
	if (*arr)
	{
		while (arr[count])
			count++;
	}
	return (count);
}

static int	envp_len(t_vars *myenv)
{
	int		size;

	size = 0;
	while (myenv)
	{
		size++;
		myenv = myenv->next;
	}
	return (size);
}

int	safe_arr_push(char ***arr, const char *str)
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
