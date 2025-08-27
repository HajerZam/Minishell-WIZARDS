/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:18:15 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/27 08:58:32 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*extract_key(const char *env_var)
{
	char	*equal_pos;
	size_t	key_len;

	equal_pos = ft_strchr(env_var, '=');
	if (!equal_pos)
		return (ft_strdup(env_var));
	key_len = equal_pos - env_var;
	return (ft_substr(env_var, 0, key_len));
}

static char	*extract_value(const char *env_var)
{
	char	*equal_pos;

	equal_pos = ft_strchr(env_var, '=');
	if (!equal_pos)
		return (ft_strdup(""));
	return (ft_strdup(equal_pos + 1));
}

static int	create_env_node(t_env **env, char *env_str, t_env **new_node)
{
	*new_node = malloc(sizeof(t_env));
	if (!*new_node)
	{
		free_env(*env);
		return (0);
	}
	(*new_node)->key = extract_key(env_str);
	(*new_node)->value = extract_value(env_str);
	(*new_node)->is_exported = 1;
	(*new_node)->next = NULL;
	if (!(*new_node)->key || !(*new_node)->value)
	{
		free((*new_node)->key);
		free((*new_node)->value);
		free(*new_node);
		free_env(*env);
		return (0);
	}
	return (1);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*new_node;
	int		i;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		if (!create_env_node(&env, envp[i], &new_node))
			return (NULL);
		new_node->next = env;
		env = new_node;
		i++;
	}
	return (env);
}
