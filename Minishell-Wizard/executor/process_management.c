/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:46:27 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/08 19:46:27 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	get_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	else
		return (1);
}

static int	wait_single_process(t_exec_context *ctx, int i, int cmd_count)
{
	int	status;
	int	last_status;

	if (ctx->pids[i] > 0)
	{
		if (waitpid(ctx->pids[i], &status, 0) == -1)
		{
			if (errno != ECHILD)
				perror("waitpid");
		}
		else
		{
			last_status = get_exit_status(status);
			if (i == cmd_count - 1)
				ctx->last_exit_status = last_status;
		}
	}
	return (0);
}

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
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}

void	handle_signals_in_parent(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
}
