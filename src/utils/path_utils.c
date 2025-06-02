/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:02:46 by ggalizon          #+#    #+#             */
/*   Updated: 2025/06/02 13:02:47 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**get_paths(t_vars *myenv)
{
	char	*rawpath;
	char	**paths;

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
	char	*fullpath;
	int		i;

	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	i = -1;
	paths = get_paths(myenv);
	if (!(paths))
		return (NULL);
	while (paths[++i])
	{
		fullpath = ft_triplestrjoin(paths[i], "/", cmd);
		if (access(fullpath, F_OK) == 0)
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

int	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}
