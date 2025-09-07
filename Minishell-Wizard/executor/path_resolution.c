/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:47:11 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/06 22:08:55 by halzamma         ###   ########.fr       */
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

static char	*create_full_path(char *dir, char *command)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, command);
	free(temp);
	return (full_path);
}

static char	*check_path_directory(char *dir, char *command, char **paths)
{
	char	*full_path;

	full_path = create_full_path(dir, command);
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
	return (NULL);
}

static char	*search_in_path_dirs(char **paths, char *command)
{
	char	*result;
	int		i;

	if (!paths || !command)
	{
		free_argv(paths);
		return (NULL);
	}
	i = 0;
	while (paths[i])
	{
		result = check_path_directory(paths[i], command, paths);
		if (result)
			return (result);
		i++;
	}
	free_argv(paths);
	return (NULL);
}

char *find_command_path_env(char *command, t_env *env)
{
    char **paths;
    char *path_env;
    
    if (!command)
        return (NULL);
    if (ft_strchr(command, '/'))
        return (handle_absolute_path(command));
    path_env = getenv_from_list(env, "PATH");
    if (!path_env || *path_env == '\0')
        return (NULL);
    paths = ft_split(path_env, ':');
    if (!paths)
    {
        print_execution_error(command, "command not found");
        return (NULL);
    }
    return (search_in_path_dirs(paths, command));
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
