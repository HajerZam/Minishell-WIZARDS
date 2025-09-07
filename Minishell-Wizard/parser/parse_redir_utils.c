/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 18:49:18 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 18:50:49 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	has_input_redirection(t_cmd *cmd)
{
	return (cmd->is_heredoc || cmd->input_fd != 0);
}

int	has_output_redirection(t_cmd *cmd)
{
	return (cmd->output_fd != 1);
}
