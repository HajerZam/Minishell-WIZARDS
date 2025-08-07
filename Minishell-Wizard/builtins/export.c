/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:06:59 by halzamma          #+#    #+#             */
/*   Updated: 2025/07/29 12:06:59 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Print all environment variables in sorted order with export format */
static void	print_export_list(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		printf("declare -x %s=\"%s\"\n", current->key, current->value);
		current = current->next;
	}
}

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

/* Parse key=value format and extract key and value */
static int	parse_export_arg(const char *arg, char **key, char **value)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
	{
		*key = ft_strdup(arg);
		*value = NULL;
		return (*key != NULL);
	}
	*key = ft_substr(arg, 0, equal_pos - arg);
	*value = ft_strdup(equal_pos + 1);
	return (*key != NULL && *value != NULL);
}

/* Handle single export argument */
static int	handle_export_arg(t_env **env, const char *arg)
{
	char	*key;
	char	*value;
	int		success;

	if (!parse_export_arg(arg, &key, &value))
	{
		printf("export: memory allocation failed\n");
		return (1);
	}
	if (!is_valid_identifier(key))
	{
		printf("export: `%s': not a valid identifier\n", arg);
		free(key);
		free(value);
		return (1);
	}
	if (value)
	{
		if (update_env_value(env, key, value))
			success = 0;
		else if (add_env_var(env, key, value))
			success = 0;
		else
		{
			printf("export: failed to set variable %s\n", key);
			success = 1;
		}
	}
	else
	{
		if (!getenv_from_list(*env, key))
		{
			if (!add_env_var(env, key, ""))
			{
				printf("export: failed to set variable %s\n", key);
				success = 1;
			}
			else
				success = 0;
		}
		else
			success = 0;
	}
	free(key);
	free(value);
	return (success);
}

/* Main export builtin function */
int	builtin_export(char **args, t_env **env)
{
	int	i;
	int	exit_status;

	if (!args[1])
	{
		print_export_list(*env);
		return (0);
	}
	exit_status = 0;
	i = 1;
	while (args[i])
	{
		if (handle_export_arg(env, args[i]))
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
