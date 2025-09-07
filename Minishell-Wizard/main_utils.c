/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 15:59:24 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 20:27:36 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	restore_backups(t_exec_context *ctx)
{
	if (ctx->stdin_backup != -1)
	{
		dup2(ctx->stdin_backup, STDIN_FILENO);
		close(ctx->stdin_backup);
	}
	if (ctx->stdout_backup != -1)
	{
		dup2(ctx->stdout_backup, STDOUT_FILENO);
		close(ctx->stdout_backup);
	}
}

void	cleanup_resources(t_exec_context *ctx, t_env *env)
{
	if (ctx)
	{
		cleanup_execution_context(ctx);
		if (env)
			free_env(env);
		if (ctx->exported_env)
			free_env(ctx->exported_env);
		restore_backups(ctx);
	}
	rl_clear_history();
}

void	cleanup_and_exit(t_exec_context *ctx, t_env *env, int exit_status)
{
	if (ctx)
		cleanup_execution_context(ctx);
	if (env)
		free_env(env);
	rl_clear_history();
	printf("Goodbye from WizardShell!\n");
	exit(exit_status);
}

int	process_tokens(char *expanded_input, t_exec_context *ctx)
{
	t_token	*tokens;
	t_cmd	*cmd_list;

	tokens = tokenize_and_check_signal(expanded_input, ctx);
	if (!tokens)
		return (1);
	cmd_list = parse_and_check_signal(tokens, expanded_input, ctx);
	if (!cmd_list)
		return (1);
	if (handle_final_signal_check(cmd_list, ctx))
		return (1);
	return (execute_command_or_exit(cmd_list, ctx));
}

void	handle_readline_interruption(t_exec_context *ctx)
{
	if (g_signal_received == SIGINT)
	{
		ctx->last_exit_status = 130;
		g_signal_received = 0;
	}
}
