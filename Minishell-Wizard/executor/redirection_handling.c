/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handling.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:45:45 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/08 19:45:45 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * setup_redirections - Set up input/output redirections for a command
 * cmd: Command with redirection information
 * Return: 0 on success, 1 on error
 */
int	setup_redirections(t_cmd *cmd)
{
	if (!cmd)
		return (1);
	if (cmd->input_fd != 0)
	{
		if (dup2(cmd->input_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			return (1);
		}
		close(cmd->input_fd);
	}
	if (cmd->output_fd != 1)
	{
		if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			return (1);
		}
		close(cmd->output_fd);
	}

	return (0);
}

/**
 * backup_std_fds - Backup standard file descriptors
 * ctx: Execution context
 * Return: 0 on success, 1 on error
 */
int	backup_std_fds(t_exec_context *ctx)
{
	ctx->stdin_backup = dup(STDIN_FILENO);
	ctx->stdout_backup = dup(STDOUT_FILENO);
	if (ctx->stdin_backup == -1 || ctx->stdout_backup == -1)
	{
		perror("dup");
		return (1);
	}

	return (0);
}

/**
 * restore_std_fds - Restore standard file descriptors
 * ctx: Execution context
 * Return: 0 on success, 1 on error
 */
int	restore_std_fds(t_exec_context *ctx)
{
	int	result;

    result = 0;
	if (ctx->stdin_backup != -1)
	{
		if (dup2(ctx->stdin_backup, STDIN_FILENO) == -1)
		{
			perror("dup2");
			result = 1;
		}
		close(ctx->stdin_backup);
		ctx->stdin_backup = -1;
	}
	if (ctx->stdout_backup != -1)
	{
		if (dup2(ctx->stdout_backup, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			result = 1;
		}
		close(ctx->stdout_backup);
		ctx->stdout_backup = -1;
	}
	return (result);
}
