#include "includes/minishell.h"

char	**get_paths(char **envp)
{
	char	*rawpath;
	char	**paths;
	int		i;

	i = 0;
	rawpath = NULL;
	while (*envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH", 4))
		{
			rawpath = ft_strnstr(envp[i], "=", 5) + 1;
			break ;
		}
		i++;
	}
	paths = ft_split(rawpath, ':');
	return (paths);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	**paths;
	char	*basepath;
	char	*fullpath;
	int		i;

	i = -1;
	paths = get_paths(envp);
	while (paths[++i])
	{
		basepath = ft_strjoin(paths[i], "/");
		fullpath = ft_strjoin(basepath, cmd);
		free(basepath);
		basepath = NULL;
		if (access(fullpath, X_OK) == 0)
		{
			flee_arr((void **)paths);
			return (fullpath);
		}
		free(fullpath);
		fullpath = NULL;
	}
	if (fullpath)
		free(fullpath);
	free_arr((void **)paths);
	return (NULL);
}
