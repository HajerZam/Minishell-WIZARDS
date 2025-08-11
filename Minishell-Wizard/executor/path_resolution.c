/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:47:11 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/08 19:47:11 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * find_command_path - Find the full path to a command
 * command: Command name to search for
 * envp: Environment variables array
 * Return: Full path to command or NULL if not found
 */
char	*find_command_path(char *command, char **envp)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	char	*temp;
	int		i;

	if (!command)
		return (NULL);
	if (ft_strchr(command, '/'))
	{
		if (is_executable_file(command))
			return (ft_strdup(command));
		return (NULL);
	}
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
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

		if (full_path && is_executable_file(full_path))
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

/**
 * is_executable_file - Check if a file exists and is executable
 * path: Path to check
 * Return: 1 if executable, 0 otherwise
 */
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
