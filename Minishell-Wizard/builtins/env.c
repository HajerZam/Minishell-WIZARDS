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

static void print_env_list(t_env *env)
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

// FIXED: Consistent signature - takes only t_env*
int builtin_env(t_env *env)
{
    if (!env)
    {
        return (0);
    }
    print_env_list(env);
    return (0);
}