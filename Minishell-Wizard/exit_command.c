/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 18:21:39 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/09 14:43:04 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_exit_args(t_cmd *cmd_list, t_exec_context *ctx)
{
	if (cmd_list->argv[1] && cmd_list->argv[2])
	{
		ft_putstr_fd("wizardshell: exit: too many arguments\n", 2);
		ctx->last_exit_status = 1;
		free_cmd_list(cmd_list);
		return (1);
	}
	return (0);
}

static int	parse_exit_status(char *arg)
{
	char	*endptr;
	long	val;

	if (!arg)
		return (0);
	val = strtol(arg, &endptr, 10);
	if (*endptr != '\0')
	{
		ft_putstr_fd("wizardshell: exit: numeric argument required\n", 2);
		return (2);
	}
	return ((int)val);
}

int	handle_exit_command(t_cmd *cmd_list, t_exec_context *ctx)
{
	int	exit_status;

	if (validate_exit_args(cmd_list, ctx))
		return (1);
	printf("exit\n");
	exit_status = parse_exit_status(cmd_list->argv[1]);
	free_cmd_list(cmd_list);
	ctx->last_exit_status = exit_status;
	return (0);
}
