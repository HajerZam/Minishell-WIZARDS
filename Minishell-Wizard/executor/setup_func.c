/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_func.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 13:33:23 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/23 13:33:23 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	setup_pipe_fds(t_cmd *cmd, t_exec_context *ctx, int cmd_index)
{
	if (setup_input_pipe(cmd, ctx, cmd_index) != 0)
		return (1);
	if (setup_output_pipe(cmd, ctx, cmd_index) != 0)
		return (1);
	close_all_pipe_fds(ctx);
	return (0);
}
