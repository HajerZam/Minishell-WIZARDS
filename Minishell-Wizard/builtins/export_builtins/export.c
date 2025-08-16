/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:06:59 by fepennar          #+#    #+#             */
/*   Updated: 2025/08/15 21:40:12 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/* Print all environment variables in sorted order with export format
	Happens when export is sent with no args */
static void	print_export_list(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (current->is_exported)
		{
			printf("declare -x %s", current->key);
			if (current->value)
				printf("=\"%s\"", current->value);
			printf("\n");
		}
		current = current->next;
	}
	return ;
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

/*Exports an existing variable without changing it's value, (ONLY VAR)
 * If the variable exists, it marks it as exported.
 * If it doesn't exist, it creates a new (empty) exported variable*/
int export_existing_var(const char *key, t_env **env)
{
    t_env *var_node;

    var_node = find_env_var(*env, key);
    if (var_node)
    {
        var_node->is_exported = 1;
        return (1);
    }
    return (add_env_var_exported(env, key, NULL));
}

/* Handle single export argument */
static int	handle_export_arg(t_env **env, const char *arg)
{
	char	*key;
	char	*value;

	if (ft_strchr(arg, '=') != NULL)
	{
		if(!parse_assignment(arg, &key, &value))
			return (1);
		if (!is_valid_identifier(key))
		{
			printf("minishell: export: `%s': not a valid identifier\n", key);
			free(key);
			free(value);
			return (1);
		}
		export_with_assignment(key, value, env);
		free(key);
		free(value);
	}
	else
	{
		if (!is_valid_identifier(arg))
		{
			printf("minishell: export: `%s': not a valid identifier\n", arg);
			return (1);
		}
		export_existing_var(arg, env);
	}
	return (0);
}

/* Main export function returns 1 on error */
int	builtin_export(char **args, t_env *env)
{
	int	i;
	int	exit_status;
	t_env **env_ptr;

	env_ptr = &env;
	if (!args[1])
	{
		print_export_list(env);
		return (0);
	}
	exit_status = 0;
	i = 1;
	while (args[i])
	{
		if (handle_export_arg(env_ptr, args[i]))
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
