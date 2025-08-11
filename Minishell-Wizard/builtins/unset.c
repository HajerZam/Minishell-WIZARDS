/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:07:12 by halzamma          #+#    #+#             */
/*   Updated: 2025/07/29 12:07:12 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_env_index(char **envp, const char *name)
{
	int i;
	size_t len;

	if (!envp || !name)
		return (-1);
	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int builtin_unset(char **argv, t_env *env)
{
	int i;
	int index;

	if (!argv || !env)
		return (1);
	i = 1;
	while (argv[i])
	{
		index = find_env_index(env->envp, argv[i]);
		if (index != -1)
		{
			free(env->envp[index]);
			env->envp[index] = NULL;
			while (env->envp[index + 1])
			{
				env->envp[index] = env->envp[index + 1];
				index++;
			}
			env->envp[index] = NULL;
		}
		i++;
	}
	return (0);
}
