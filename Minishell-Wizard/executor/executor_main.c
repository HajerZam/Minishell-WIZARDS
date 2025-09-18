/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:41:13 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/18 21:26:24 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_sigint_during_pipeline(t_exec_context *ctx, t_cmd *cmd_list)
{
	if (g_signal_received == SIGINT)
	{
		activate_ctx_functions(ctx, cmd_list, 0);
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		init_signals_interactive();
		return (1);
	}
	return (0);
}

int	execute_pipeline(t_cmd *cmd_list, t_exec_context *ctx)
{
	int	cmd_count;
	int	heredoc_result;

	if (!cmd_list || !ctx)
		return (1);
	if (check_global_signal(ctx))
		return (130);
	heredoc_result = handle_heredoc_processing(cmd_list, ctx);
	if (heredoc_result)
		return (heredoc_result);
	cmd_count = count_commands(cmd_list);
	ctx->pipe_count = cmd_count - 1;
	if (cmd_count == 1)
		return (handle_single_command(cmd_list, ctx));
	if (setup_pipeline(cmd_list, ctx) != 0)
		return (1);
	if (handle_sigint_during_pipeline(ctx, cmd_list))
		return (130);
	activate_ctx_functions(ctx, cmd_list, 1);
	return (ctx->last_exit_status);
}

int	handle_builtin_no_pipe(t_cmd *cmd, t_exec_context *ctx)
{
	int	original_stdin;
	int	original_stdout;
	int	status;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdin == -1 || original_stdout == -1)
	{
		perror("dup");
		return (1);
	}
	if (setup_redirections(cmd) != 0)
	{
		close(original_stdin);
		close(original_stdout);
		return (1);
	}
	status = execute_builtin(cmd, &ctx->env);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
	ctx->last_exit_status = status;
	return (status);
}
