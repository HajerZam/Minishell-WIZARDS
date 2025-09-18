/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exec_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 21:03:40 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/18 21:13:00 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	cleanup_remaining_heredocs(t_cmd *current)
{
	while (current && current->next)
	{
		current = current->next;
		if (current->heredoc_delimiter)
		{
			free(current->heredoc_delimiter);
			current->heredoc_delimiter = NULL;
		}
	}
}

int	handle_heredoc_redirection(t_cmd *cmd, const char *delimiter,
	t_exec_context *ctx)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (!cmd || !delimiter)
	{
		ft_putstr_fd("wizardshell: heredoc: ", 2);
		ft_putstr_fd("invalid command or delimiter\n", 2);
		return (0);
	}
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (0);
	}
	if (pid == 0)
		heredoc_child(delimiter, pipe_fd);
	return (heredoc_parent(cmd, pipe_fd, pid, ctx));
}
