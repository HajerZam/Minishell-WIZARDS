/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 16:06:39 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 20:11:23 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_signal_in_process(t_exec_context *ctx)
{
	ctx->last_exit_status = 130;
	g_signal_received = 0;
	return (1);
}

static int	process_valid_input(char *input, t_exec_context *ctx)
{
	int	continue_loop;

	add_history(input);
	if (g_signal_received == SIGINT)
		return (handle_signal_in_process(ctx));
	continue_loop = process_command_line(input, ctx);
	if (continue_loop == 0)
		return (0);
	return (1);
}

int	process_input_line(char *input, t_exec_context *ctx)
{
	if (g_signal_received == SIGINT)
		return (handle_signal_in_process(ctx));
	if (*input && g_signal_received == 0)
		return (process_valid_input(input, ctx));
	return (1);
}

int	init_shell(t_exec_context *ctx, t_env **env, char **envp)
{
	*env = create_env_copy_direct(envp);
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

char	*get_complete_input(void)
{
	char	*complete_input;

	complete_input = get_initial_input();
	if (!complete_input)
		return (NULL);
	while (has_unclosed_quotes(complete_input))
	{
		if (g_signal_received == SIGINT)
		{
			free(complete_input);
			return (NULL);
		}
		complete_input = handle_multiline_input(complete_input);
		if (!complete_input)
			return (NULL);
	}
	return (complete_input);
}
