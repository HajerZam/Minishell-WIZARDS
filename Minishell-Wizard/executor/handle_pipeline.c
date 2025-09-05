/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 13:42:33 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/28 15:59:31 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_ctx_list(t_exec_context *ctx, t_cmd *cmd_list)
{
	if (ctx->pids)
		free(ctx->pids);
	if (ctx->env)
		free_env(ctx->env);
	if (cmd_list)
		free_cmd_list(cmd_list);
}

/* Updated handle_child_process */
void	handle_child_process(t_cmd *cmd, t_exec_context *ctx, t_cmd *cmd_list)
{
	char	*command_path;

	handle_signals_in_child();
	if (setup_redirections(cmd) != 0)
	{
		cleanup_child_process(ctx, cmd_list);
		exit(1);
	}
	command_path = find_command_path(cmd->argv[0]);
	if (!command_path)
	{
		print_execution_error(cmd->argv[0], "command not found");
		cleanup_child_process(ctx, cmd_list);
		exit(127);
	}
	if (execve(command_path, cmd->argv, ctx->envp) == -1)
	{
		perror(command_path);
		free(command_path);
		cleanup_child_process(ctx, cmd_list);
		exit(126);
	}
	free(command_path);
	cleanup_child_process(ctx, cmd_list);
}

void	handle_parent_process(pid_t pid, t_exec_context *ctx)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		ctx->last_exit_status = 1;
		return ;
	}
	if (WIFEXITED(status))
		ctx->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ctx->last_exit_status = 128 + WTERMSIG(status);
	else
		ctx->last_exit_status = 1;
}

/* Updated handle_pipeline_child */
static void	execute_child_command(t_cmd *cmd, t_exec_context *ctx,\
	t_cmd *cmd_list)
{
	char	*command_path;
	int		exit_status;

	if (cmd->is_builtin)
	{
		exit_status = execute_builtin(cmd, &ctx->env);
		cleanup_child_process(ctx, cmd_list);
		exit(exit_status);
	}
	command_path = find_command_path(cmd->argv[0]);
	if (!command_path)
	{
		print_execution_error(cmd->argv[0], "command not found");
		cleanup_child_process(ctx, cmd_list);
		exit(127);
	}
	if (execve(command_path, cmd->argv, ctx->envp) == -1)
	{
		perror(command_path);
		free(command_path);
		cleanup_child_process(ctx, cmd_list);
		exit(126);
	}
}

void	handle_pipeline_child(t_cmd *cmd, t_exec_context *ctx,\
	int cmd_index, t_cmd *cmd_list)
{
	handle_signals_in_child();
	if (setup_pipe_fds(cmd, ctx, cmd_index) != 0)
	{
		cleanup_child_process(ctx, cmd_list);
		exit(1);
	}
	if (setup_redirections(cmd) != 0)
	{
		cleanup_child_process(ctx, cmd_list);
		exit(1);
	}
	execute_child_command(cmd, ctx, cmd_list);
}

