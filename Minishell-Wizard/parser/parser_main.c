/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 14:29:31 by fepennar          #+#    #+#             */
/*   Updated: 2025/08/11 15:32:04 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_double_pipe(t_token *current)
{
	while (current)
	{
		if (current->type == PIPE)
		{
			if (!current->next)
				return (0);
			if (current->next->type == PIPE)
				return (0);
		}
		if (is_redirection(current->type))
		{
			if (!current->next || current->next->type != WORD)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

void	init_parser(t_parser *parser, t_token *tokens)
{
	parser->tokens = tokens;
	parser->current = tokens;
	parser->cmd_list = NULL;
	parser->error = 0;
}

// FIXED: Better syntax validation
static int	check_syntax_errors(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (!current)
		return (1);
	if (current->type == PIPE)
	{
		fprintf(stderr, "bash: syntax error near unexpected token `|'\n");
		return (0);
	}
	if (check_double_pipe(current))
	{
		fprintf(stderr, "bash: syntax error near unexpected token `|'\n");
		return (0);
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
	return (check_syntax_errors(tokens));
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
		fprintf(stderr, "bash: syntax error near unexpected token\n");
		free_cmd_list(result);
		return (NULL);
	}
	if (parser.error)
	{
		free_cmd_list(result);
		return (NULL);
	}
	return (result);
}
