#include "../includes/minishell.h"

char	**get_paths(t_vars *myenv)
{
	char	*rawpath;
	char	**paths;
	int		i;

	i = 0;
	rawpath = NULL;
	paths = NULL;
	while (myenv)
	{
		if (is_equal(myenv->key, "PATH"))
		{
			rawpath = myenv->value;
			break ;
		}
		myenv = myenv->next;
	}
	if (rawpath)
		paths = ft_split(rawpath, ':');
	return (paths);
}

char	*get_cmd_path(char *cmd, t_vars *myenv)
{
	char	**paths;
	char	*basepath;
	char	*fullpath;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	i = -1;
	if (!(paths = get_paths(myenv)))
		return NULL;
	while (paths[++i])
	{
		//TODO make a strjoin for 3 strings then we can shorten it
		//enough
		basepath = ft_strjoin(paths[i], "/");
		//TODO check if fullpath == NULL
		fullpath = ft_strjoin(basepath, cmd);
		free(basepath);
		basepath = NULL;
		if (access(fullpath, X_OK) == 0)
		{
			free_arr((void **)paths);
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
