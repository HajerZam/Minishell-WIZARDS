/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:02:17 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/08 20:02:17 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/**
 * count_commands - Count the number of commands in a pipeline
 * cmd_list: Head of command list
 * Return: Number of commands
 */
int	count_commands(t_cmd *cmd_list)
{
	t_cmd	*current;
	int		count;

	current = cmd_list;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/**
 * init_execution_context - Initialize execution context
 * ctx: Execution context to initialize
 * envp: Environment variables
 * Return: 0 on success, 1 on error
 */
int	init_execution_context(t_exec_context *ctx, char **envp)
{
	if (!ctx)
		return (1);

	ctx->envp = envp;
	ctx->last_exit_status = 0;
	ctx->pipe_count = 0;
	ctx->pipes = NULL;
	ctx->pids = NULL;
	ctx->stdin_backup = -1;
	ctx->stdout_backup = -1;

	return (0);
}

/**
 * cleanup_execution_context - Clean up execution context
 * ctx: Execution context to clean up
 */
void	cleanup_execution_context(t_exec_context *ctx)
{
	if (!ctx)
		return;

	cleanup_pipes(ctx);

	if (ctx->stdin_backup != -1)
	{
		close(ctx->stdin_backup);
		ctx->stdin_backup = -1;
	}

	if (ctx->stdout_backup != -1)
	{
		close(ctx->stdout_backup);
		ctx->stdout_backup = -1;
	}
}

/**
 * print_execution_error - Print execution error message
 * command: Command that failed
 * error: Error message
 */
void	print_execution_error(char *command, char *error)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\n", 2);
}
