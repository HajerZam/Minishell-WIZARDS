/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 12:06:59 by fepennar          #+#    #+#             */
/*   Updated: 2025/08/26 16:12:54 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*extract_var_name(const char *str)
{
	char	*eq_position;
	int		key_len;

	eq_position = ft_strchr(str, '=');
	if (!eq_position)
		return (ft_strdup(str));
	key_len = eq_position - str;
	return (ft_substr(str, 0, key_len));
}

char	*extract_var_value(const char *str)
{
	char	*eq_position;

	eq_position = ft_strchr(str, '=');
	if (!eq_position)
		return (NULL);
	return (ft_strdup(eq_position + 1));
}


/* Searches for a specific variable in the env list */
t_env	*find_env_var(t_env *env, const char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

/* Creates a new variable and allocates memory for the node key and value
   and adds the node on top of the list*/
int	add_env_var_exported(t_env **env, const char *key, const char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (0);
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->is_exported = 1;
	if (!new_node->key || (value && !new_node->value))
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (0);
	}
	new_node->next = *env;
	*env = new_node;
	return (1);
}

/* Export a variable with assignment (VAR=value)
 if it exists it updates the value and the flag, if not
 It creates a New Exported Variable */
int	export_with_assignment(const char *key, const char *value, t_env **env)
{
    t_env *existing;

    existing = find_env_var(*env, key);
    if (existing)
    {
        free(existing->value);
        if (value)
            existing->value = ft_strdup(value);
        else
            existing->value = NULL;
        existing->is_exported = 1;
        return (1);
    }
    return (add_env_var_exported(env, key, value));
}
