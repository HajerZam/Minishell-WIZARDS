/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:47:11 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/28 10:05:03 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*handle_absolute_path(char *command)
{
	if (is_directory(command))
	{
		print_execution_error(command, "is a directory");
		return (NULL);
	}
	if (is_executable_file(command))
		return (ft_strdup(command));
	print_execution_error(command, "No such file or directory");
	return (NULL);
}

static char	*search_in_path_dirs(char **paths, char *command)
{
	char	*full_path;
	char	*temp;
	int		i;

	if (!paths || !command)
	{
		free_argv(paths);
		return (NULL);
	}
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
		{
			free_argv(paths);
			return (NULL);
		}
		full_path = ft_strjoin(temp, command);
		free(temp);
		if (!full_path)
		{
			free_argv(paths);
			return (NULL);
		}
		if (is_executable_file(full_path))
		{
			free_argv(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_argv(paths);
	return (NULL);

}

char	*find_command_path(char *command)
{
	char	**paths;
	char	*path_env;

	if (!command)
		return (NULL);
	if (ft_strchr(command, '/'))
		return (handle_absolute_path(command));
	path_env = getenv("PATH");
	if (!path_env)
	{
		print_execution_error(command, "No such file or directory");
		return (NULL);
	}
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (search_in_path_dirs(paths, command));
}

int	is_executable_file(char *path)
{
	struct stat	st;

	if (!path)
		return (0);
	if (stat(path, &st) == 0)
	{
		if (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
			return (1);
	}
	return (0);
}
