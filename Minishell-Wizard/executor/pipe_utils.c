/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 12:21:35 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 09:55:43 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	allocate_pipe_arrays(t_exec_context *ctx, int cmd_count)
{
	int	i;
	
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
	while (i < cmd_count)
	{
		ctx->pids[i] = -1;
		i++;
	}
	return (0);
}

void	initialize_pipes_array(t_exec_context *ctx, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		ctx->pipes[i] = NULL;
		i++;
	}
}

int	create_pipe_fds(t_exec_context *ctx, int cmd_count)
{
	int	i;

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
