/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fepennar <fepennar@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 14:29:31 by fepennar          #+#    #+#             */
/*   Updated: 2025/07/17 14:29:31 by fepennar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_parser(t_parser *parser, t_token *tokens)
{
	parser->tokens = tokens;
	parser->current = tokens;
	parser->cmd_list = NULL;
	parser->error = 0;
}

static int	check_redirection_errors(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == REDIR_APPEND || current->type == HEREDOC)
		{
			if (!current->next || current->next->type != WORD)
			{
				fprintf(stderr,
					"Syntax error: expected filename after redirection\n");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}
static int	check_pipe_errors(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == PIPE)
		{
			if (!current->next || current->next->type == PIPE)
			{
				fprintf(stderr, "Syntax error: unexpected pipe\n");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

int	validate_syntax(t_token *tokens)
{
	if (!tokens)
	{
		fprintf(stderr, "No tokens to validate\n");
		return (0);
	}
	if (!check_pipe_errors(tokens))
	{
		fprintf(stderr, "Syntax error: unexpected pipe\n");
		return (0);
	}
	if (!check_redirection_errors(tokens))
	{
		fprintf(stderr, "Syntax error: invalid redirection\n");
		return (0);
	}
	return (1);
}

t_cmd	*parse_command_line(t_token *tokens)
{
	t_parser	parser;
	t_cmd		*result;

	if (!tokens)
		return (NULL);
	if (!validate_syntax(tokens))
		return (NULL);
	init_parser(&parser, tokens);
	result = parse_pipeline(&parser);
	if (parser.current && !parser.error)
	{
		fprintf(stderr, "Parsing error occurred\n");
		free_cmd_list(result);
		return (NULL);
	}
	if (parser.error)
	{
		fprintf(stderr, "Parser error\n");
		free_cmd_list(result);
		return (NULL);
	}
	return (result);
}
