/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_assignment.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 09:58:19 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/10 14:22:01 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/* Validate variable name - 
	must start with letter/underscore, contain only alphanumeric/underscore
*/
static int	is_valid_identifier(const char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	handle_export_assignment(t_env **env, const char *arg)
{
	char	*key;
	char	*value;
	int		is_append;

	key = extract_var_name(arg);
	value = extract_var_value(arg);
	is_append = is_append_assignment(arg);
	if (!is_valid_identifier(key))
	{
		if (is_append)
			printf("wizardshell: export: `%s+=%s': not a valid identifier\n",
				key, value);
		else
			printf("wizardshell: export: `%s=%s': not a valid identifier\n",
				key, value);
		free(key);
		free(value);
		return (1);
	}
	if (is_append)
	{
		if (!export_with_append(key, value, env))
		{
			free(key);
			free(value);
			return (1);
		}
	}
	else
	{
		if (!export_with_assignment(key, value, env))
		{
			free(key);
			free(value);
			return (1);
		}
	}
	free(key);
	free(value);
	return (0);
}

int	handle_export_simple(t_env **env, const char *arg)
{
	if (!is_valid_identifier(arg))
	{
		printf("wizardshell: export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	export_existing_var(arg, env);
	return (0);
}
