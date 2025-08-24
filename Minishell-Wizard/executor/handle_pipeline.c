/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 13:42:33 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/23 13:42:33 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_child_process(t_cmd *cmd, t_exec_context *ctx)
{
	char	*command_path;

	handle_signals_in_child();
	if (setup_redirections(cmd) != 0)
		exit(1);
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

void	handle_pipeline_child(t_cmd *cmd, t_exec_context *ctx, int cmd_index)
{
	char	*command_path;

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
