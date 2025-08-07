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

/* Validate variable name for unset - same rules as export */
static int	is_valid_unset_name(const char *name)
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

/* Main unset builtin function
	unset_env_var returns 1 if found and removed, 0 if not found
	Both cases are success for unset command
*/
int	builtin_unset(char **args, t_env **env)
{
	int	i;
	int	exit_status;

	if (!args[1])
		return (0);
	exit_status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_unset_name(args[i]))
		{
			printf("unset: `%s': not a valid identifier\n", args[i]);
			exit_status = 1;
		}
		else
		{
			unset_env_var(env, args[i]);
		}
		i++;
	}
	return (exit_status);
}
