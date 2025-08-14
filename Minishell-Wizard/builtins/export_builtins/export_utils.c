/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fepennar <fepennar@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 12:06:59 by fepennar          #+#    #+#             */
/*   Updated: 2025/08/13 12:06:59 by fepennar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
/* Separates a (VAR=value) assignment in key and value and allocates
   Memory for both separetely (KEW AND VALUE MUST BE FREED)*/
int	parse_assignment(const char *arg, char **key, char **value)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
		return (0);
	*key = ft_substr(arg, 0, equal_pos - arg);
	if (!*key)
		return (0);
	*value = ft_strdup(equal_pos + 1);
	if (!*value)
	{
		free(*key);
		return (0);
	}
	return (1);
}
/* Validates an identificator and prints error if it isn't valid*/
int	validate_export_name(const char *name)
{
	int	i;

	i = 1;
	if (!name || !*name)
	{
		printf("minishell: export: `%s': not a valid identifier\n", name);
		return (0);
	}
	if (!ft_isalpha(name[0]) && name[0] != '_')
	{
		printf("minishell: export: `%s': not a valid identifier\n", name);
		return (0);
	}
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
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
	new_node->value = ft_strdup(value);
	new_node->is_exported = 1;
	if (!new_node->value || !new_node->key)
	{
		free(new_node->key);
		free(new_node);
		free(new_node->value);
		return (0);
	}
	new_node->next = *env;
	*env = new_node;
	return (1);
}
/* Export a variable with assignmente (VAR=value)
 if it exists it updates the value and the flag, if not
 It creates a New Exported Variable */

int	export_with_assignment(const char *key, const char *value, t_env **env)
{
	t_env	*existing;

	existing = find_env_var(*env, key);
	if (existing)
	{
		free(existing->value);
		existing->value = ft_strdup(value);
		if (!existing->value)
			return (0);
		existing->is_exported = 1;
		return (existing->value != NULL);
	}
	return (add_env_var_exported(env, key, value));
}
