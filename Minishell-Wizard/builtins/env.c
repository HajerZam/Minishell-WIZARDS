/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:06:52 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/27 14:29:31 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_env_list(t_env *env)
{
	while (env)
	{
		if (env->is_exported && env->value != NULL)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int	builtin_env(t_env *env)
{
	if (!env)
		return (0);
	print_env_list(env);
	return (0);
}
