#include "includes/minishell.h"

char	*get_rawpath(char **envp)
{
	char	*rawpath;
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
	return (rawpath);
}

char	*get_cmd_path(char *cmd, char *rawpath)
{
	char	**paths;
	char	*basepath;
	char	*fullpath;
	int		i;

	i = -1;
	paths = ft_split(rawpath, ':');
	while (paths[++i])
	{
		basepath = ft_strjoin(paths[i], "/");
		fullpath = ft_strjoin(basepath, cmd);
		free(basepath);
		basepath = NULL;
		if (access(fullpath, X_OK) == 0)
		{
			free_array((void **)paths);
			return (fullpath);
		}
		free(fullpath);
		fullpath = NULL;
	}
	free(fullpath);
	fullpath = NULL;
	free_array((void **)paths);
	return (NULL);
}

void	process(t_command *cmd)
{
	pid_t	pid;
	pid = fork();
	if (pid == 0)
	{
//		cmd = get_cmd_path(args[0], get_rawpath(envp)
		execve(cmd->name, cmd->arguments, NULL);
	}
//	if (cmd)
//		execve(cmd, &args[0], envp);
	free(cmd);
	perror("Command not found");
}
