/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 18:03:50 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 18:05:30 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_expansion_error(t_exec_context *ctx)
{
	ft_putstr_fd("wizardshell: expansion error\n", 2);
	ctx->last_exit_status = 1;
	init_signals_interactive();
	return (1);
}

static int	handle_signal_interrupt(char *expanded_input, t_exec_context *ctx)
{
	ctx->last_exit_status = 130;
	g_signal_received = 0;
	free(expanded_input);
	init_signals_interactive();
	return (1);
}

int	process_command_line(char *input, t_exec_context *ctx)
{
	char	*expanded_input;
	int		result;

	if (!input || !*input)
		return (1);
	if (g_signal_received == SIGINT)
	{
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		return (1);
	}
	init_signals_execution();
	expanded_input = expand_variables(input, ctx->env, ctx->last_exit_status);
	if (!expanded_input)
		return (handle_expansion_error(ctx));
	if (g_signal_received == SIGINT)
		return (handle_signal_interrupt(expanded_input, ctx));
	result = process_tokens(expanded_input, ctx);
	init_signals_interactive();
	return (result);
}
