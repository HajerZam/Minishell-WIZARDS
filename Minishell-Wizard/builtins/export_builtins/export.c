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


/* Main export function returns 1 on error */
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
		if (ft_strchr(args[i], '=') != NULL)
		{
			if (handle_export_assignment(env, args[i]))
				exit_status = 1;
		}
		else
		{
			if (handle_export_simple(env, args[i]))
				exit_status = 1;
		}
		i++;
	}
	return (exit_status);
}
