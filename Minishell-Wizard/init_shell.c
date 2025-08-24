/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 16:06:39 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/24 16:06:39 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_input_line(char *input, t_env *env, t_exec_context *ctx)
{
	int	continue_loop;

	if (*input && g_signal_received == 0)
	{
		add_history(input);
		continue_loop = process_command_line(input, env, ctx);
		if (continue_loop == 0)
			return (0);
	}
	return (1);
}

int	init_shell(t_exec_context *ctx, t_env **env, char **envp)
{
	*env = init_env(envp);
	if (!*env)
	{
		ft_putstr_fd("WizardShell: failed to initialize environment\n", 2);
		return (1);
	}
	if (setup_execution_context(ctx, *env, envp) != 0)
	{
		ft_putstr_fd("WizardShell: failed to initialize execution context\n",
			2);
		free_env(*env);
		return (1);
	}
	return (0);
}
