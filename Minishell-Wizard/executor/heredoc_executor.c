/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:51:43 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/18 21:22:00 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	process_heredoc_line(const char *delimiter, int delim_len,
	int pipe_fd[2])
{
	char	*line;

	line = readline("heredoc> ");
	if (!line)
	{
		ft_putstr_fd("wizardshell: warning: ", 2);
		ft_putstr_fd("here-document delimited by end-of-file\n", 2);
		return (1);
	}
	if (ft_strncmp(line, delimiter, delim_len + 1) == 0)
	{
		free(line);
		return (1);
	}
	ft_putendl_fd(line, pipe_fd[1]);
	free(line);
	return (0);
}

int	heredoc_child(const char *delimiter, int pipe_fd[2])
{
	int		delim_len;

	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	close(pipe_fd[0]);
	delim_len = ft_strlen(delimiter);
	while (1)
	{
		if (process_heredoc_line(delimiter, delim_len, pipe_fd))
			break ;
	}
	close(pipe_fd[1]);
	exit(0);
}

int	handle_heredoc_wait(pid_t pid, int pipe_fd[2])
{
	int		status;
	void	(*saved_sigint)(int);
	void	(*saved_sigquit)(int);

	saved_sigint = signal(SIGINT, SIG_IGN);
	saved_sigquit = signal(SIGQUIT, SIG_IGN);
	close(pipe_fd[1]);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		signal(SIGINT, saved_sigint);
		signal(SIGQUIT, saved_sigquit);
		return (0);
	}
	signal(SIGINT, saved_sigint);
	signal(SIGQUIT, saved_sigquit);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipe_fd[0]);
		return (0);
	}
	return (1);
}

int	heredoc_parent(t_cmd *cmd, int pipe_fd[2], pid_t pid,
	t_exec_context *ctx)
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
					current->heredoc_delimiter, ctx);
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
