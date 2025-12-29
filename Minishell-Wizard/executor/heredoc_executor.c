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

void	free_cmd_list_except_delimiter(t_cmd *cmd_list,
		const char *keep_delimiter)
{
	t_cmd	*current;
	t_cmd	*next;
	int		i;

	current = cmd_list;
	while (current)
	{
		next = current->next;
		if (current->argv)
		{
			i = -1;
			while (current->argv[++i])
				free(current->argv[i]);
			free(current->argv);
		}
		if (current->heredoc_delimiter
			&& current->heredoc_delimiter != keep_delimiter)
			free(current->heredoc_delimiter);
		if (current->input_fd > 2 && current->input_fd != -1)
			close(current->input_fd);
		if (current->output_fd > 2 && current->output_fd != -1)
			close(current->output_fd);
		if (current->heredoc_fd != -1)
			close(current->heredoc_fd);
		free(current);
		current = next;
	}
}

void	heredoc_child_cleanup_and_run(const char *delimiter, int pipe_fd[2],
		t_cmd *cmd_list, t_exec_context *ctx)
{
	free_cmd_list_except_delimiter(cmd_list, delimiter);
	if (ctx->env)
		free_env(ctx->env);
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	close(pipe_fd[0]);
	rl_clear_history();
	heredoc_child_process(delimiter, pipe_fd);
}

void	heredoc_child_process(const char *delimiter, int pipe_fd[2])
{
	int	delim_len;

	delim_len = ft_strlen(delimiter);
	while (1)
	{
		if (process_heredoc_line(delimiter, delim_len, pipe_fd))
			break ;
	}
	close(pipe_fd[1]);
	exit(0);
}

int	heredoc_child(char *delimiter, int pipe_fd[2])
{
	int	delim_len;

	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	close(pipe_fd[0]);
	rl_clear_history();
	delim_len = ft_strlen(delimiter);
	while (1)
	{
		if (process_heredoc_line(delimiter, delim_len, pipe_fd))
			break ;
	}
	close(pipe_fd[1]);
	free(delimiter);
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
	char	*delimiter_backup;

	current = cmd_list;
	while (current)
	{
		if (current->heredoc_delimiter)
		{
			delimiter_backup = current->heredoc_delimiter;
			result = handle_heredoc_redirection(current,
					current->heredoc_delimiter, ctx, cmd_list);
			free(delimiter_backup);
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
