/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_management.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:43:51 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/28 10:40:24 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	setup_pipes(t_exec_context *ctx, int cmd_count)
{
	int	i;

	if (cmd_count <= 1)
		return (0);
	ctx->pipes = malloc(sizeof(int *) * (cmd_count - 1));
	ctx->pids = malloc(sizeof(pid_t) * cmd_count);
	if (!ctx->pipes || !ctx->pids)
	{
		perror("malloc");
		if (ctx->pipes)
		{
			free(ctx->pipes);
			ctx->pipes = NULL;
		}
		if (ctx->pids)
		{
			free(ctx->pids);
			ctx->pids = NULL;
		}
		return (1);
	}
	i = 0;
	while (i < cmd_count - 1)
	{
		ctx->pipes[i] = NULL;
		i++;
	}
	i = 0;
	while (i < cmd_count - 1)
	{
		ctx->pipes[i] = malloc(sizeof(int) * 2);
		if (!ctx->pipes[i])
		{
			perror("malloc");
			cleanup_pipes(ctx);
			return (1);
		}
		if (pipe(ctx->pipes[i]) == -1)
		{
			perror("pipe");
			cleanup_pipes(ctx);
			return (1);
		}
		i++;
	}
	return (0);
}

int	setup_input_pipe(t_cmd *cmd, t_exec_context *ctx, int cmd_index)
{
	if (cmd_index > 0 && cmd->input_fd == 0)
	{
		if (dup2(ctx->pipes[cmd_index - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2 stdin");
			return (1);
		}
	}
	return (0);
}

int	setup_output_pipe(t_cmd *cmd, t_exec_context *ctx, int cmd_index)
{
	if (cmd_index < ctx->pipe_count && cmd->output_fd == 1)
	{
		if (dup2(ctx->pipes[cmd_index][1], STDOUT_FILENO) == -1)
		{
			perror("dup2 stdout");
			return (1);
		}
	}
	return (0);
}
