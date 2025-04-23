#include "includes/minishell.h"

int	pwd(void)
{
	int		status;
	char	pwd[PATH_MAX];

	status = 0;
	if (getcwd(pwd, PATH_MAX) != NULL)
		printf("%s\n", pwd);
	else
		status = 1;
	return (status);
}
