/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:46:27 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 09:54:28 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
static int	wait_single_process(t_exec_context *ctx, int i, int cmd_count)
{
	int		status;
	int		last_status;
	pid_t	result;

	if (ctx->pids[i] <= 0)
		return (0);
	while (1)
	{
		result = waitpid(ctx->pids[i], &status, 0);
		if (result == ctx->pids[i])
		{
			last_status = get_exit_status(status);
			if (i == cmd_count - 1)
				ctx->last_exit_status = last_status;
			break ;
		}
		else if (result == -1)
		{
			if (errno == EINTR)
			{
				if (g_signal_received == SIGINT)
				{
					ctx->last_exit_status = 130;
					g_signal_received = 0;
				}
				continue ;
			}
			if (errno != ECHILD)
				perror("waitpid");
			break ;
		}
	}
	return (0);
}
*/

int	wait_for_processes(t_exec_context *ctx)
{
	int	i;
	int	cmd_count;

	if (!ctx->pids)
		return (0);
	cmd_count = ctx->pipe_count + 1;
	i = 0;
	while (i < cmd_count)
	{
		wait_single_process(ctx, i, cmd_count);
		i++;
	}
	return (0);
}

void	handle_signals_in_child(void)
{
	init_signals_child();
}

void	handle_signals_in_parent(void)
{
	init_signals_execution();
}
