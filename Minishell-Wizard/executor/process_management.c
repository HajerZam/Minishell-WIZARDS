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

/**
 * wait_for_processes - Wait for all child processes to complete
 * ctx: Execution context
 * Return: 0 on success, 1 on error
 */
int	wait_for_processes(t_exec_context *ctx)
{
	int		i;
	int		status;
	int		cmd_count;

	if (!ctx->pids)
		return (0);

	cmd_count = ctx->pipe_count + 1;
	i = 0;
	while (i < cmd_count)
	{
		if (ctx->pids[i] > 0)
		{
			if (waitpid(ctx->pids[i], &status, 0) == -1)
			{
				perror("waitpid");
				return (1);
			}
			if (i == cmd_count - 1)
			{
				if (WIFEXITED(status))
					ctx->last_exit_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					ctx->last_exit_status = 128 + WTERMSIG(status);
			}
		}
		i++;
	}

	return (0);
}

/**
 * handle_signals_in_child - Set up signal handling for child processes
 */
void	handle_signals_in_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * handle_signals_in_parent - Set up signal handling for parent process
 */
void	handle_signals_in_parent(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
