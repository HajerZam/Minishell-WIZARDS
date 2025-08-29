/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_management.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:43:51 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/29 12:22:31 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	setup_pipes(t_exec_context *ctx, int cmd_count)
{
	if (cmd_count <= 1)
		return (0);
	if (allocate_pipe_arrays(ctx, cmd_count) != 0)
		return (1);
	initialize_pipes_array(ctx, cmd_count);
	if (create_pipe_fds(ctx, cmd_count) != 0)
		return (1);
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
