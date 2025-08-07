#include "minishell.h"

int	handle_heredoc_redirection(t_cmd *cmd, const char *delimiter)
{
	if (!cmd || !delimiter)
	{
		fprintf(stderr, "Invalid command or filename for heredoc\n");
		return (0);
	}
	cmd->heredoc_fd = -4; // Placeholder for heredoc file descriptor
	cmd->is_heredoc = 1;  // Set flag indicating this command uses heredoc
	return (1);
}

int	handle_input_redirection(t_cmd *cmd, const char *filename)
{
	int	fd;

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
	if (!cmd || !filename)
		return (0);
	if (append == 0)
		cmd->output_fd = -3;
	else if (append == 1)
		cmd->output_fd = -2;
	return (1);
}

int	case_redirection(t_cmd *cmd, t_token_type redir_type, char *filename)
{
	if (redir_type == REDIR_IN)
	{
		if (!cmd || !filename)
			return (0);
		cmd->input_fd = -2;
		return (1);
	}
	else if (redir_type == REDIR_OUT)
		return (handle_output_redirection(cmd, filename, 0));
	else if (redir_type == REDIR_APPEND)
		return (handle_output_redirection(cmd, filename, 1));
	else if (redir_type == HEREDOC)
		return (handle_heredoc_redirection(cmd, filename));
	else
	{
		fprintf(stderr, "Unknown redirection type\n");
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
		fprintf(stderr, "Expected a filename after redirection\n");
		parser->error = 1;
		return (0);
	}
	filename = parser->current->value;
	consume_token(parser);
	return (case_redirection(cmd, redir_type, filename));
}
