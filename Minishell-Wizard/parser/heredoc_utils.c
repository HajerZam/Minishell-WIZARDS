/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:03:58 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/29 13:09:33 by halzamma         ###   ########.fr       */
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

static void	heredoc_child(const char *delimiter, int pipe_fd[2])
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	close(pipe_fd[0]);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, pipe_fd[1]);
		free(line);
	}
	close(pipe_fd[1]);
	exit(0);
}

static int	heredoc_parent(t_cmd *cmd, int pipe_fd[2], pid_t pid)
{
	int		status;
	void	(*saved_sig)(int);

	saved_sig = signal(SIGINT, SIG_IGN);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	signal(SIGINT, saved_sig);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipe_fd[0]);
		ft_putstr_fd("\n", 1);
		return (0);
	}
	heredoc_cleanup(cmd, pipe_fd);
	return (1);
}

int	handle_heredoc_redirection(t_cmd *cmd, const char *delimiter)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (heredoc_check(cmd, delimiter, pipe_fd) == 0)
	{
		ft_putstr_fd("heredoc: error initializing heredoc\n", 2);
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
	return (heredoc_parent(cmd, pipe_fd, pid));
}
