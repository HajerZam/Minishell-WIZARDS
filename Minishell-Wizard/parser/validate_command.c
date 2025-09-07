/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 12:48:56 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 18:51:38 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	validate_parsed_command(t_cmd *cmd, t_parser *parser)
{
	if (!cmd)
	{
		parser->error = 1;
		return (0);
	}
	if ((!cmd->argv || !cmd->argv[0]))
	{
		if (has_input_redirection(cmd) || has_output_redirection(cmd))
			return (1);
		ft_putstr_fd("wizardshell: command not found\n", 2);
		parser->error = 1;
		return (0);
	}
	if (ft_strlen(cmd->argv[0]) == 0)
	{
		ft_putstr_fd("wizardshell: command not found\n", 2);
		parser->error = 1;
		return (0);
	}
	return (1);
}
