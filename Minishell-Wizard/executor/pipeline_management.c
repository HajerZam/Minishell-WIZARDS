/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_management.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:43:51 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/08 19:43:51 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * setup_pipes - Create pipes for pipeline communication
 * ctx: Execution context
 * cmd_count: Number of commands in pipeline
 * Return: 0 on success, 1 on error
 */

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
		return (1);
	}
	i = 0;
	while (i < cmd_count - 1)
	{
		ctx->pipes[i] = malloc(sizeof(int) * 2);
		if (!ctx->pipes[i])
		{
			perror("malloc");
			return (1);
		}
		if (pipe(ctx->pipes[i]) == -1)
		{
			perror("pipe");
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * cleanup_pipes - Close and free pipe resources
 * ctx: Execution context
 */
void	cleanup_pipes(t_exec_context *ctx)
{
	int	i;

	if (!ctx->pipes)
		return;

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

/**
 * setup_pipe_fds - Set up file descriptors for piping
 * cmd: Current command
 * ctx: Execution context
 * cmd_index: Index of current command
 * Return: 0 on success, 1 on error
 */
int	setup_pipe_fds(t_cmd *cmd, t_exec_context *ctx, int cmd_index)
{
    int    i;

	if (cmd_index > 0 && cmd->input_fd == 0)
	{
		if (dup2(ctx->pipes[cmd_index - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			return (1);
		}
	}
	if (cmd_index < ctx->pipe_count && cmd->output_fd == 1)
	{
		if (dup2(ctx->pipes[cmd_index][1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			return (1);
		}
	}
	i = 0;
	while (i < ctx->pipe_count)
	{
		close(ctx->pipes[i][0]);
		close(ctx->pipes[i][1]);
		i++;
	}
	return (0);
}
