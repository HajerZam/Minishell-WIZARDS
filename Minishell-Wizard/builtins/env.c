/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:06:52 by halzamma          #+#    #+#             */
/*   Updated: 2025/07/29 12:06:52 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Print all environment variables in key=value format */
static void	print_env_list(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

/* Main env builtin function 
	Basic env implementation - just print all environment variables
	bash env has more options (-i, -u, etc.) but this covers basic functionality
	env with arguments not supported in basic implementation
*/
int	builtin_env(char **args, t_env **env)
{
	if (args[1])
	{
		printf("env: arguments not supported in this implementation\n");
		return (1);
	}
	print_env_list(*env);
	return (0);
}
