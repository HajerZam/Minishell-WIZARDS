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
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		else
			printf("%s=\n", env->key);
		env = env->next;
	}
}

/* Main env builtin function 
	Basic env implementation - just print all environment variables
	bash env has more options (-i, -u, etc.) but this covers basic functionality
	env with arguments not supported in basic implementation
*/
int	builtin_env(char **args, t_env **env)
{
	if (args && args[0])
	{
		fprintf(stderr, "env: '%s': No such file or directory\n", args[0]);
		return (1);
	}
	if (!env || !*env)
	{
		fprintf(stderr, "env: No environment variables set\n");
		return (1);
	}
	print_env_list(*env);
	return (0);
}
