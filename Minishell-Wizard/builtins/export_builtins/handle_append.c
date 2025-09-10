/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_append.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:15:34 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/10 14:19:47 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/* Check if the assignment uses the += operator */
int	is_append_assignment(const char *str)
{
	char	*append_position;
	char	*eq_position;

	append_position = ft_strstr(str, "+=");
	eq_position = ft_strchr(str, '=');
	return (append_position && (!eq_position || append_position < eq_position));
}

int	export_with_append(const char *key, const char *append_value, t_env **env)
{
	t_env	*existing;
	char	*new_value;

	if (!append_value)
		append_value = "";
	existing = find_env_var(*env, key);
	if (existing)
	{
		if (existing->value)
		{
			new_value = ft_strjoin(existing->value, append_value);
			if (!new_value)
				return (0);
			free(existing->value);
			existing->value = new_value;
		}
		else
		{
			existing->value = ft_strdup(append_value);
			if (!existing->value)
				return (0);
		}
		existing->is_exported = 1;
		return (1);
	}
	return (add_env_var_exported(env, key, append_value));
}
