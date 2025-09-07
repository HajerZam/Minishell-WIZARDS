/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 13:42:33 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 09:52:05 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_child_process(t_cmd *cmd, t_exec_context *ctx, t_cmd *cmd_list)
{
	char	*command_path;
	char	**current_envp;

	handle_signals_in_child();
	if (setup_redirections(cmd) != 0)
	{
		cleanup_child_process(ctx, cmd_list);
		exit(1);
	}
	command_path = find_command_path_env(cmd->argv[0], ctx->env);
	check_cmd(cmd, ctx, cmd_list, command_path);
	current_envp = env_list_to_array(ctx->env);
	check_envp(ctx, cmd_list, command_path, current_envp);
	if (execve(command_path, cmd->argv, current_envp) == -1)
	{
		free(command_path);
		free_env_array(current_envp, -1);
		cleanup_child_process(ctx, cmd_list);
		exit(126);
	}
}

static void	update_exit_status(int status, t_exec_context *ctx)
{
	int	sig;

	if (WIFEXITED(status))
		ctx->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			ctx->last_exit_status = 130;
		else if (sig == SIGQUIT)
		{
			ctx->last_exit_status = 131;
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		}
		else
			ctx->last_exit_status = 128 + sig;
	}
	else
		ctx->last_exit_status = 1;
}

void	handle_parent_process(pid_t pid, t_exec_context *ctx)
{
	int		status;
	pid_t	result;
	int		done;

	status = 0;
	done = 0;
	while (!done)
	{
		result = waitpid(pid, &status, 0);
		if (result == pid)
		{
			update_exit_status(status, ctx);
			done = 1;
		}
		else if (result == -1)
		{
			if (errno == EINTR)
				continue ;
			perror("waitpid");
			ctx->last_exit_status = 1;
			done = 1;
		}
	}
}

static void	execute_child_command(t_cmd *cmd, t_exec_context *ctx,
		t_cmd *cmd_list)
{
	char	*command_path;
	char	**current_envp;
	int		exit_status;

	if (cmd->is_builtin)
	{
		exit_status = execute_builtin(cmd, &ctx->env);
		cleanup_child_process(ctx, cmd_list);
		exit(exit_status);
	}
	command_path = find_command_path_env(cmd->argv[0], ctx->env);
	check_cmd(cmd, ctx, cmd_list, command_path);
	current_envp = env_list_to_array(ctx->env);
	check_envp(ctx, cmd_list, command_path, current_envp);
	if (execve(command_path, cmd->argv, current_envp) == -1)
	{
		free(command_path);
		free_env_array(current_envp, -1);
		cleanup_child_process(ctx, cmd_list);
		exit(126);
	}
}

void	handle_pipeline_child(t_cmd *cmd, t_exec_context *ctx, int cmd_index,
		t_cmd *cmd_list)
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
