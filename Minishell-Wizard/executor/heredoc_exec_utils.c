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
	t_exec_context *ctx, t_cmd *cmd_list)
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
		heredoc_child_cleanup_and_run(delimiter, pipe_fd, cmd_list, ctx);
	return (heredoc_parent(cmd, pipe_fd, pid, ctx));
}

int	heredoc_parent(t_cmd *cmd, int pipe_fd[2], pid_t pid, t_exec_context *ctx)
{
	if (!handle_heredoc_wait(pid, pipe_fd))
	{
		ctx->last_exit_status = 130;
		g_signal_received = SIGINT;
		return (0);
	}
	if (cmd->input_fd != 0)
		close(cmd->input_fd);
	cmd->input_fd = pipe_fd[0];
	cmd->is_heredoc = 1;
	return (1);
}

int	process_heredocs(t_cmd *cmd_list, t_exec_context *ctx)
{
	t_cmd	*current;
	int		result;

	current = cmd_list;
	while (current)
	{
		if (current->heredoc_delimiter)
		{
			result = handle_heredoc_redirection(current,
					current->heredoc_delimiter, ctx, cmd_list);
			free(current->heredoc_delimiter);
			current->heredoc_delimiter = NULL;
			if (!result || g_signal_received == SIGINT)
			{
				cleanup_remaining_heredocs(current);
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}
