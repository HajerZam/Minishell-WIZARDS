/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 13:37:57 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/29 12:17:29 by halzamma         ###   ########.fr       */
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
		result = execute_single_command(current, ctx, cmd_index, cmd_list);
		current = current->next;
		cmd_index++;
	}
	return (result);
}

static char	*validate_command_path(t_cmd *cmd, t_exec_context *ctx)
{
	char	*command_path;

	command_path = find_command_path(cmd->argv[0]);
	if (!command_path)
	{
		print_execution_error(cmd->argv[0], "command not found");
		ctx->last_exit_status = 127;
		return (NULL);
	}
	return (command_path);
}

int	execute_external_single(t_cmd *cmd, t_exec_context *ctx)
{
	pid_t	pid;
	char	*command_path;

	command_path = validate_command_path(cmd, ctx);
	if (!command_path)
		return (127);
	free(command_path);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		handle_child_process(cmd, ctx, cmd);
	else
		handle_parent_process(pid, ctx);
	return (ctx->last_exit_status);
}

int	execute_single_command(t_cmd *cmd, t_exec_context *ctx, int cmd_index, \
	t_cmd *cmd_list)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	return (execute_external(cmd, ctx, cmd_index, cmd_list));
}
