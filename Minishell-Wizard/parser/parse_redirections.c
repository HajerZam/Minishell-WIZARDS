// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   parse_redirections.c                               :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/08/07 14:59:25 by halzamma          #+#    #+#             */
// /*   Updated: 2025/08/29 13:10:37 by halzamma         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../minishell.h"

// int	handle_heredoc_redirection(t_cmd *cmd, const char *delimiter)
// {
// 	char	*line;
// 	int		pipe_fd[2];

// 	if (heredoc_check(cmd, delimiter, pipe_fd) == 0)
// 	{
// 		ft_putstr_fd("heredoc: error initializing heredoc\n", 2);
// 		return (0);
// 	}
	
// 	line = readline("heredoc> ");
// 	while (line)
// 	{
// 		if (ft_strcmp(line, delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		write(pipe_fd[1], line, ft_strlen(line));
// 		write(pipe_fd[1], "\n", 1);
// 		free(line);
// 		printf("heredoc> ");
// 		line = readline("");
// 	}
// 	heredoc_cleanup(cmd, pipe_fd);
// 	return (1);


// }

// int	handle_input_redirection(t_cmd *cmd, const char *filename)
// {
// 	int	fd;

// 	if (!cmd || !filename)
// 	{
// 		ft_putstr_fd("Invalid command or filename for input redirection\n", 2);
// 		return (0);
// 	}
// 	fd = open(filename, O_RDONLY);
// 	if (fd == -1)
// 	{
// 		perror(filename);
// 		return (0);
// 	}
// 	if (cmd->input_fd != 0)
// 		close(cmd->input_fd);
// 	cmd->input_fd = fd;
// 	return (1);
// }

// int	handle_output_redirection(t_cmd *cmd, const char *filename, int append)
// {
// 	int	fd;
// 	int	flags;

// 	if (!cmd || !filename)
// 	{
// 		ft_putstr_fd("Invalid command or filename for output redirection\n", 2);
// 		return (0);
// 	}
// 	flags = O_WRONLY | O_CREAT;
// 	if (append)
// 		flags |= O_APPEND;
// 	else
// 		flags |= O_TRUNC;
// 	fd = open(filename, flags, 0644);
// 	if (fd == -1)
// 	{
// 		perror(filename);
// 		return (0);
// 	}
// 	if (cmd->output_fd != 1)
// 		close(cmd->output_fd);
// 	cmd->output_fd = fd;
// 	return (1);
// }

// int	case_redirection(t_cmd *cmd, t_token_type redir_type, char *filename)
// {
// 	if (redir_type == REDIR_IN)
// 		return (handle_input_redirection(cmd, filename));
// 	else if (redir_type == REDIR_OUT)
// 		return (handle_output_redirection(cmd, filename, 0));
// 	else if (redir_type == REDIR_APPEND)
// 		return (handle_output_redirection(cmd, filename, 1));
// 	else if (redir_type == HEREDOC)
// 		return (handle_heredoc_redirection(cmd, filename));
// 	else
// 	{
// 		ft_putstr_fd("Unknown redirection type\n", 2);
// 		return (0);
// 	}
// }

// int	parse_redirections(t_parser *parser, t_cmd *cmd)
// {
// 	t_token_type	redir_type;
// 	char			*filename;

// 	if (!is_redirection(parser->current->type))
// 	{
// 		ft_putstr_fd("Expected redirection token\n", 2);
// 		parser->error = 1;
// 		return (0);
// 	}
// 	redir_type = parser->current->type;
// 	consume_token(parser);
// 	if (!parser->current || parser->current->type != WORD)
// 	{
// 		ft_putstr_fd("wizardshell: syntax error near unexpected token\n", 2);
// 		parser->error = 1;
// 		return (0);
// 	}
// 	filename = parser->current->value;
// 	consume_token(parser);
// 	return (case_redirection(cmd, redir_type, filename));
// }



/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:59:25 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 12:55:00 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
** HEREDOC
** Legge righe fino al delimitatore e collega la read-end della pipe a cmd->input_fd.
** Ritorna 1 su successo, 0 su errore.
*/
int	handle_heredoc_redirection(t_cmd *cmd, const char *delimiter)
{
	char	*line;
	int		pipe_fd[2];

	if (heredoc_check(cmd, delimiter, pipe_fd) == 0)
	{
		ft_putstr_fd("heredoc: error initializing heredoc\n", 2);
		return (0);
	}
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
<<<<<<< HEAD
=======
		line = readline("heredoc> ");
>>>>>>> 54e6c374f4feb0d87f9be62ff93bb610efbc2412
	}
	heredoc_cleanup(cmd, pipe_fd);
	return (1);
}

/*
** < infile
*/
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

/*
** > outfile   (append == 0)
** >> outfile  (append == 1)
*/
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

/*
** Smista la singola redirezione al relativo handler.
*/
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
	ft_putstr_fd("Unknown redirection type\n", 2);
	return (0);
}

/*
** parse_redirections:
** Consuma TUTTE le redirezioni in sequenza: (REDIR WORD)*
** Ritorna 1 su successo, 0 su errore (e setta parser->error).
*/
// int	parse_redirections(t_parser *parser, t_cmd *cmd)
// {
// 	t_token_type	redir_type;
// 	char			*filename;

// 	while (parser->current && is_redirection(parser->current->type))
// 	{
// 		redir_type = parser->current->type;
// 		consume_token(parser);
// 		if (!parser->current || parser->current->type != WORD)
// 		{
// 			ft_putstr_fd("wizardshell: syntax error near unexpected token\n", 2);
// 			parser->error = 1;
// 			return (0);
// 		}
// 		filename = parser->current->value;
// 		consume_token(parser);
// 		if (!case_redirection(cmd, redir_type, filename))
// 		{
// 			parser->error = 1;
// 			return (0);
// 		}
// 	}
// 	return (1);
// }
int	parse_redirections(t_parser *parser, t_cmd *cmd)
{
	t_token_type	redir_type;
	char			*filename;

	while (parser->current && is_redirection(parser->current->type))
	{
		/* salva operatore e consuma */
		redir_type = parser->current->type;
		consume_token(parser);

		/* dopo un operatore DEVE arrivare una WORD (delimitatore o filename) */
		if (!parser->current || parser->current->type != WORD)
		{
			ft_putstr_fd("wizardshell: syntax error near unexpected token\n", 2);
			parser->error = 1;
			return (0);
		}
		filename = parser->current->value;
		consume_token(parser);

		/* applica la redirezione; se fallisce, marca errore parser */
		if (!case_redirection(cmd, redir_type, filename))
		{
			parser->error = 1;
			return (0);
		}
	}
	return (1);
}

int	has_input_redirection(t_cmd *cmd)
{
	return (cmd->is_heredoc || cmd->input_fd != 0);
}

int	has_output_redirection(t_cmd *cmd)
{
	return (cmd->output_fd != 1);
}
