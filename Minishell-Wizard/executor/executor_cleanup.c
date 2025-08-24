/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cleanup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:35:22 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/11 15:35:22 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cleanup_execution_context(t_exec_context *ctx)
{
	if (!ctx)
		return ;
	if (ctx->pipes)
		cleanup_pipes(ctx);
	if (ctx->pids)
	{
		free(ctx->pids);
		ctx->pids = NULL;
	}
	if (ctx->stdin_backup != -1)
	{
		close(ctx->stdin_backup);
		ctx->stdin_backup = -1;
	}
	if (ctx->stdout_backup != -1)
	{
		close(ctx->stdout_backup);
		ctx->stdout_backup = -1;
	}
	ctx->pipe_count = 0;
	ctx->envp = NULL;
	ctx->env = NULL;
}

void	cleanup_pipes(t_exec_context *ctx)
{
	int	i;

	if (!ctx->pipes)
		return ;
	i = 0;
	while (i < ctx->pipe_count)
	{
		if (ctx->pipes[i])
		{
			close(ctx->pipes[i][0]);
			close(ctx->pipes[i][1]);
			free(ctx->pipes[i]);
		}
		i++;
	}
	free(ctx->pipes);
	ctx->pipes = NULL;
	if (ctx->pids)
	{
		free(ctx->pids);
		ctx->pids = NULL;
	}
}

void	close_all_pipe_fds(t_exec_context *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->pipe_count)
	{
		if (ctx->pipes[i])
		{
			close(ctx->pipes[i][0]);
			close(ctx->pipes[i][1]);
		}
		i++;
	}
}
