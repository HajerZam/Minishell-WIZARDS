/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 13:37:57 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/23 13:37:57 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	setup_pipeline(t_cmd *cmd_list, t_exec_context *ctx)
{
	int	cmd_count;

	cmd_count = count_commands(cmd_list);
	ctx->pipe_count = cmd_count - 1;
	if (backup_std_fds(ctx) != 0)
		return (1);
	if (setup_pipes(ctx, cmd_count) != 0)
	{
		restore_std_fds(ctx);
		return (1);
	}
	return (0);
}

int	execute_pipeline_commands(t_cmd *cmd_list, t_exec_context *ctx)
{
	t_cmd	*current;
	int		cmd_index;
	int		result;

	current = cmd_list;
	cmd_index = 0;
	result = 0;
	while (current && result == 0)
	{
		result = execute_single_command(current, ctx, cmd_index);
		current = current->next;
		cmd_index++;
	}
	return (result);
}

int	execute_external_single(t_cmd *cmd, t_exec_context *ctx)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		handle_child_process(cmd, ctx);
	else
		handle_parent_process(pid, ctx);
	return (ctx->last_exit_status);
}

int	execute_single_command(t_cmd *cmd, t_exec_context *ctx, int cmd_index)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	return (execute_external(cmd, ctx, cmd_index));
}

int	execute_external(t_cmd *cmd, t_exec_context *ctx, int cmd_index)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		handle_pipeline_child(cmd, ctx, cmd_index);
	else
	{
		if (ctx->pids && cmd_index < ctx->pipe_count + 1)
			ctx->pids[cmd_index] = pid;
	}
	return (0);
}
