/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:05:53 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/18 21:14:54 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_heredoc_only_command(t_cmd *cmd, t_exec_context *ctx)
{
	if (cmd->heredoc_delimiter)
	{
		if (!handle_heredoc_redirection(cmd, cmd->heredoc_delimiter, ctx, cmd))
			return (1);
		free(cmd->heredoc_delimiter);
		cmd->heredoc_delimiter = NULL;
	}
	return (0);
}

int	execute_single_command_no_pipe(t_cmd *cmd, t_exec_context *ctx)
{
	int	result;

	if (!cmd)
		return (1);
	if (g_signal_received == SIGINT)
	{
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		return (130);
	}
	if ((!cmd->argv || !cmd->argv[0]) && (cmd->heredoc_delimiter
			|| has_input_redirection(cmd) || has_output_redirection(cmd)))
	{
		result = handle_heredoc_only_command(cmd, ctx);
		ctx->last_exit_status = result;
		return (result);
	}
	if (!cmd->argv || !cmd->argv[0])
		return (1);
	if (cmd->is_builtin)
		return (handle_builtin_no_pipe(cmd, ctx));
	init_signals_execution();
	result = execute_external_single(cmd, ctx);
	init_signals_interactive();
	return (result);
}
