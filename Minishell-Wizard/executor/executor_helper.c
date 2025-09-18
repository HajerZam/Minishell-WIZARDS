/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 21:19:32 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/18 21:28:01 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_global_signal(t_exec_context *ctx)
{
	if (g_signal_received == SIGINT)
	{
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		return (1);
	}
	return (0);
}

void	activate_ctx_functions(t_exec_context *ctx, t_cmd *cmd_list,
		int in_signal)
{
	if (in_signal == 1)
	{
		execute_pipeline_commands(cmd_list, ctx);
		close_all_pipe_fds(ctx);
		wait_for_processes(ctx);
	}
	cleanup_pipes(ctx);
	restore_std_fds(ctx);
	if (in_signal == 1)
		init_signals_interactive();
}

int	handle_heredoc_processing(t_cmd *cmd_list, t_exec_context *ctx)
{
	if (!process_heredocs(cmd_list, ctx))
	{
		if (g_signal_received == SIGINT)
		{
			ctx->last_exit_status = 130;
			g_signal_received = 0;
			return (130);
		}
		return (1);
	}
	return (0);
}

int	handle_single_command(t_cmd *cmd_list, t_exec_context *ctx)
{
	return (execute_single_command_no_pipe(cmd_list, ctx));
}
