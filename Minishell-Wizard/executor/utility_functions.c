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

void	print_execution_error(char *command, char *error)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\n", 2);
}

void	set_exit_status(t_exec_context *ctx, int status)
{
	if (ctx)
		ctx->last_exit_status = status;
}

int	is_directory(char *path)
{
	struct stat	st;

	if (!path)
		return (0);
	if (stat(path, &st) == 0)
		return (S_ISDIR(st.st_mode));
	return (0);
}
