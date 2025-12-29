/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:59:25 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/18 19:56:38 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_input_redirection(t_cmd *cmd, const char *filename)
{
	int	fd;

	if (!cmd || !filename)
	{
		ft_putstr_fd("Invalid command or filename for input redirection\n", 2);
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
		ft_putstr_fd("Invalid command or filename for output redirection\n", 2);
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
	{
		if (cmd->heredoc_delimiter)
			free(cmd->heredoc_delimiter);
		cmd->heredoc_delimiter = ft_strdup(filename);
		if (!cmd->heredoc_delimiter)
		{
			ft_putstr_fd("Memory allocation failed for heredoc delimiter\n", 2);
			return (0);
		}
		return (cmd->heredoc_delimiter != NULL);
	}
	ft_putstr_fd("Unknown redirection type\n", 2);
	return (0);
}

int	parse_redirections(t_parser *parser, t_cmd *cmd)
{
	t_token_type	redir_type;
	char			*filename;

	while (parser->current && is_redirection(parser->current->type))
	{
		redir_type = parser->current->type;
		consume_token(parser);
		if (!parser->current || parser->current->type != WORD)
		{
			ft_putstr_fd("wizardshell: syntax error near unexpected token\n",
				2);
			parser->error = 1;
			return (0);
		}
		filename = parser->current->value;
		consume_token(parser);
		if (!case_redirection(cmd, redir_type, filename))
		{
			parser->error = 1;
			return (0);
		}
	}
	return (1);
}
