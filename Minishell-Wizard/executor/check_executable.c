/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_executable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 12:12:10 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/29 12:12:10 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_executable_file(char *path)
{
	struct stat	st;

	if (!path)
		return (0);
	if (stat(path, &st) == 0)
	{
		if (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
			return (1);
	}
	return (0);
}

static int	handle_non_builtin_command(t_cmd *cmd, t_exec_context *ctx)
{
	char	*command_path;

	if (ctx->pipe_count == 0)
	{
		command_path = find_command_path(cmd->argv[0]);
		if (!command_path)
		{
			print_execution_error(cmd->argv[0], "command not found");
			ctx->last_exit_status = 127;
			return (127);
		}
		free(command_path);
	}
	return (0);
}

int	execute_external(t_cmd *cmd, t_exec_context *ctx, int cmd_index, t_cmd *cmd_list)
{
	pid_t	pid;

	if (!cmd->is_builtin)
	{
		if (handle_non_builtin_command(cmd, ctx) != 0)
			return (ctx->last_exit_status);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		handle_pipeline_child(cmd, ctx, cmd_index, cmd_list);
	else
	{
		if (ctx->pids && cmd_index < ctx->pipe_count + 1)
			ctx->pids[cmd_index] = pid;
	}
	return (0);
}
