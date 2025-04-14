#include "includes/minishell.h"

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
//	arr = NULL; // no point in NULLing the pointer here as it's local
//	- need to do it from the code that owns the pointer!
//	"In C, arguments are passed to functions by value. 
//	This means when you call free_arr(tokens);, 
//	the arr variable inside the free_arr function is 
//	a copy of the tokens pointer from your main function (or wherever it was called).
//	When you do free(arr); inside free_arr, you are freeing the 
//	memory that the copy points to (which is the same memory 
//	the original tokens pointer points to - this is what you want).
//	However, if you were to then add arr = NULL; inside free_arr,
//	you would only be setting the local copy arr to NULL. The 
//	original tokens pointer back in the calling function remains unchanged;
//	it still holds the now-invalid memory address that was just freed.
//	Setting the local copy to NULL has no effect outside the free_arr function."
}

char	**arr_push(char ***arr, char *str)
{
	char	**new_arr;
	int	len;
	int	i;

	len = 0;
	while(*arr && (*arr)[len])
			len++;
	new_arr = (char **)malloc(sizeof(char *) * (len + 2)); //null term + new str
	if (!new_arr)
		return NULL;
	i = 0;
	while (i < len)
	{
		new_arr[i] = (*arr)[i];
		i++;
	}
	new_arr[i] = ft_strdup(str);
	if (!new_arr[i])
	{
		// There was a potential memory leak here where new_arr
		// was not being freed if ft_strdup failed
		free_arr((void **)new_arr);
		new_arr = NULL;
		return NULL;
	}
	new_arr[i + 1] = NULL;
	if (*arr)
		free(*arr);
	*arr = new_arr;
	return new_arr;
}
