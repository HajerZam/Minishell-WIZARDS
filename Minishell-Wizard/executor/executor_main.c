/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:41:13 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 09:49:39 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int execute_pipeline(t_cmd *cmd_list, t_exec_context *ctx)
{
	int	cmd_count;

	if (!cmd_list || !ctx)
		return (1);
	if (g_signal_received == SIGINT)
	{
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		return (130);
	}
	cmd_count = count_commands(cmd_list);
	ctx->pipe_count = cmd_count - 1;
	if (cmd_count == 1)
		return (execute_single_command_no_pipe(cmd_list, ctx));
		
	if (setup_pipeline(cmd_list, ctx) != 0)
		return (1);
	if (g_signal_received == SIGINT)
	{
		cleanup_pipes(ctx);
		restore_std_fds(ctx);
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		init_signals_interactive();
		return (130);
	}
	execute_pipeline_commands(cmd_list, ctx);
	close_all_pipe_fds(ctx);
	wait_for_processes(ctx);
	cleanup_pipes(ctx);
	restore_std_fds(ctx);
	init_signals_interactive();
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

int	execute_single_command_no_pipe(t_cmd *cmd, t_exec_context *ctx)
{
	int	result;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (g_signal_received == SIGINT)
	{
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		return (130);
	}
	if (cmd->is_builtin)
		return (handle_builtin_no_pipe(cmd, ctx));
	init_signals_execution();
	result = execute_external_single(cmd, ctx);
	init_signals_interactive();
	return (result);
}
