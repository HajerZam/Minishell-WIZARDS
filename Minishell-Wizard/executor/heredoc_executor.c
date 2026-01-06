/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:51:43 by halzamma          #+#    #+#             */
/*   Updated: 2025/12/29 XX:XX:XX by halzamma         ###   ########.fr       */
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

void	heredoc_child_cleanup_and_run(const char *delimiter, int pipe_fd[2],
		t_cmd *cmd_list, t_exec_context *ctx)
{
	char	delimiter_buffer[256];

	ft_strlcpy(delimiter_buffer, delimiter, sizeof(delimiter_buffer));
	free_cmd_list(cmd_list);
	if (ctx->env)
		free_env(ctx->env);
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	close(pipe_fd[0]);
	rl_clear_history();
	heredoc_child_process(delimiter_buffer, pipe_fd);
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
