#include "includes/minishell.h"

int	main()
{
	printf("must forky\n");
	char *args[] = {"touch", "aaaaa"};
	pid_t pid;
	pid = fork();

	if (pid == 0)
	{
		printf("executing forky\n");
		execve("/usr/bin/touch", args, NULL);
	}
	else
	{
		wait(NULL);
		printf("forked\n");
	}
	return (0);
}
