/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment_setup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 08:42:10 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/27 08:42:10 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	allocate_and_init_node(t_env **env_copy, char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(key);
		free(value);
		return (0);
	}
	new_node->key = key;
	new_node->value = value;
	new_node->is_exported = 1;
	new_node->envp = NULL;
	new_node->next = *env_copy;
	*env_copy = new_node;
	return (1);
}

static int	create_env_node(t_env **env_copy, char **envp, int i)
{
	char	*key;
	char	*value;
	char	*eq_pos;

	eq_pos = ft_strchr(envp[i], '=');
	if (eq_pos)
	{
		key = ft_substr(envp[i], 0, eq_pos - envp[i]);
		value = ft_strdup(eq_pos + 1);
	}
	else
	{
		key = ft_strdup(envp[i]);
		value = ft_strdup("");
	}
	if (!key || !value)
	{
		free(key);
		free(value);
		return (0);
	}
	return (allocate_and_init_node(env_copy, key, value));
}

t_env	*create_env_copy_direct(char **envp)
{
	t_env	*env_copy;
	int		i;

	env_copy = NULL;
	i = 0;
	while (envp[i])
	{
		if (!create_env_node(&env_copy, envp, i))
		{
			free_env(env_copy);
			return (NULL);
		}
		i++;
	}
	return (env_copy);
}
