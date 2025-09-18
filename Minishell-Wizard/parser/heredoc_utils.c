/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:03:58 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/18 18:58:00 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	heredoc_check(t_cmd *cmd, const char *delimiter, int pipe_fd[2])
{
	if (!cmd || !delimiter)
	{
		ft_putstr_fd("wizardshell: heredoc: invalid command or delimiter\n", 2);
		return (0);
	}
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	return (1);
}

void	heredoc_cleanup(t_cmd *cmd, int pipe_fd[2])
{
	close(pipe_fd[1]);
	if (cmd->input_fd != 0)
		close(cmd->input_fd);
	cmd->input_fd = pipe_fd[0];
	cmd->is_heredoc = 1;
}
