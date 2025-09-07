/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 18:20:12 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 18:20:24 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_eof_or_signal(t_exec_context *ctx)
{
	(void)ctx;
	if (!isatty(STDIN_FILENO))
	{
		printf("exit\n");
		return (0);
	}
	printf("exit\n");
	return (0);
}

void	cleanup_backups(t_exec_context *ctx)
{
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
