/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:59:25 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/07 15:38:32 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_heredoc_redirection(t_cmd *cmd, const char *delimiter)
{
	char	*line;
	int		pipe_fd[2];

	if (!cmd || !delimiter)
	{
		fprintf(stderr, "Invalid heredoc parameters\n");
		return (0);
	}
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	printf("heredoc> ");
	while ((line = readline("")))
	{
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
		printf("heredoc> ");
	}
	close(pipe_fd[1]);
	if (cmd->input_fd != 0)
		close(cmd->input_fd);
	cmd->input_fd = pipe_fd[0];
	cmd->is_heredoc = 1;
	return (1);
}

int	handle_input_redirection(t_cmd *cmd, const char *filename)
{
	int	fd;

	if (!cmd || !filename)
	{
		fprintf(stderr, "Invalid command or filename for input redirection\n");
		return (0);
	}
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return (0);
	}
	if (cmd->input_fd != 0)
		close(cmd->input_fd);
	cmd->input_fd = fd;
	return (1);
}

int	handle_output_redirection(t_cmd *cmd, const char *filename, int append)
{
	int	fd;
	int	flags;

	if (!cmd || !filename)
	{
		fprintf(stderr, "Invalid command or filename for output redirection\n");
		return (0);
	}
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (0);
	}
	if (cmd->output_fd != 1)
		close(cmd->output_fd);
	cmd->output_fd = fd;
	return (1);
}

int	case_redirection(t_cmd *cmd, t_token_type redir_type, char *filename)
{
	if (redir_type == REDIR_IN)
		return (handle_input_redirection(cmd, filename));
	else if (redir_type == REDIR_OUT)
		return (handle_output_redirection(cmd, filename, 0));
	else if (redir_type == REDIR_APPEND)
		return (handle_output_redirection(cmd, filename, 1));
	else if (redir_type == HEREDOC)
		return (handle_heredoc_redirection(cmd, filename));
	else
	{
		fprintf(stderr, "bash: syntax error near unexpected token\n");
		return (0);
	}
}

int	parse_redirections(t_parser *parser, t_cmd *cmd)
{
	t_token_type	redir_type;
	char			*filename;

	if (!is_redirection(parser->current->type))
	{
		fprintf(stderr, "Expected a redirection token\n");
		parser->error = 1;
		return (0);
	}
	redir_type = parser->current->type;
	consume_token(parser);
	if (!parser->current || parser->current->type != WORD)
	{
		fprintf(stderr, "bash: syntax error near unexpected token `newline'\n");
		parser->error = 1;
		return (0);
	}
	filename = parser->current->value;
	consume_token(parser);
	return (case_redirection(cmd, redir_type, filename));
}
