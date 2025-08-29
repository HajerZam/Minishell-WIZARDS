/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 12:48:56 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/29 12:48:56 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	validate_parsed_command(t_cmd *cmd, t_parser *parser)
{
	if ((!cmd->argv || !cmd->argv[0]) && (cmd->input_fd != 0
			|| cmd->output_fd != 1 || cmd->is_heredoc))
	{
		ft_putstr_fd("wizardshell: syntax error!\n", 2);
		parser->error = 1;
		return (0);
	}
	if (!cmd->argv || !cmd->argv[0])
	{
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
