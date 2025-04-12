#include "includes/minishell.h"

void	print_arr(char **arr)
{
	int	i;

	printf("printer function\n");
	i = 0;
	while(arr[i])
	{
		printf("%s\n", arr[i]);
		i++;
	}
}
void	export(t_command *cmd, char **envp)
{
	// char **envp_copy;
	// char **exports;

	printf("calling executer\n");
	// envp_copy = envp;
	if (!cmd->arguments[1])
	{
		printf("no arguments\n");
		print_arr(envp);
	}

	// array of export variables
	// copy of envp

	// key=value
	// if key -> update
	// if !key -> add key and value
	// if !value -> keep key empty
	// add key to envp

	//export alone should show all variables

	//syntax errors
}
