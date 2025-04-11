#include "includes/minishell.h"

void	print_arr(char **arr)
{
	int	i;

	i = 0;
	while(arr[i])
	{
		prinf("%s\n", arr[i]);
		i++;
	}
}
void	export(t_command *cmd, char **envp)
{
	char **envp_copy;
	char **exports;

	envp_copy = envp;
	if (!cmd->arguments[1])
	{
		print_arr(envp_copy);
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
