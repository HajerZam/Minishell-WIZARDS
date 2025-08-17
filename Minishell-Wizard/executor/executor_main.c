/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:41:13 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/08 19:41:13 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * execute_pipeline - Main function to execute a pipeline of commands
 * cmd_list: Linked list of commands to execute
 * ctx: Execution context containing environment and state
 * Return: Exit status of the last command
 */
int	execute_pipeline(t_cmd *cmd_list, t_exec_context *ctx)
{
	int		cmd_count;
	int		cmd_index;
	t_cmd	*current;

	if (!cmd_list || !ctx)
		return (1);
	cmd_count = count_commands(cmd_list);
	ctx->pipe_count = cmd_count - 1;
	if (backup_std_fds(ctx) != 0)
		return (1);
	if (cmd_count > 1 && setup_pipes(ctx, cmd_count) != 0)
	{
		restore_std_fds(ctx);
		return (1);
	}
	current = cmd_list;
	cmd_index = 0;
	while (current)
	{
		if (execute_single_command(current, ctx, cmd_index) != 0)
		{
			cleanup_pipes(ctx);
			restore_std_fds(ctx);
			return (ctx->last_exit_status);
		}
		current = current->next;
		cmd_index++;
	}
	wait_for_processes(ctx);
	cleanup_pipes(ctx);
	restore_std_fds(ctx);
	
	return (ctx->last_exit_status);
}

/**
 * execute_single_command - Execute a single command in the pipeline
 * cmd: Command to execute
 * ctx: Execution context
 * cmd_index: Index of command in pipeline
 * Return: 0 on success, 1 on error
 */
int	execute_single_command(t_cmd *cmd, t_exec_context *ctx, int cmd_index)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (cmd->is_builtin && count_commands(cmd) == 1 && ctx->pipe_count == 0)
	{
		if (setup_redirections(cmd) != 0)
			return (1);
		ctx->last_exit_status = execute_builtin(cmd, ctx->env);
		return (0);
	}
	return (execute_external(cmd, ctx, cmd_index));
}

/**
 * execute_external - Fork and execute an external command
 * cmd: Command to execute
 * ctx: Execution context
 * cmd_index: Index of command in pipeline
 * Return: 0 on success, 1 on error
 */
int	execute_external(t_cmd *cmd, t_exec_context *ctx, int cmd_index)
{
	pid_t	pid;
	char	*command_path;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		handle_signals_in_child();
		if (setup_pipe_fds(cmd, ctx, cmd_index) != 0)
			exit(1);
		if (setup_redirections(cmd) != 0)
			exit(1);
		if (cmd->is_builtin)
			exit(execute_builtin(cmd, ctx->env));
		command_path = find_command_path(cmd->argv[0]);
		if (!command_path)
		{
			print_execution_error(cmd->argv[0], "command not found");
			exit(127);
		}
		if (execve(command_path, cmd->argv, ctx->envp) == -1)
		{
			perror(command_path);
			free(command_path);
			exit(126);
		}
		free(command_path);
		exit(1);
	}
	else
	{
		if (ctx->pids && cmd_index < ctx->pipe_count + 1)
			ctx->pids[cmd_index] = pid;
	}
	
	return (0);
}
