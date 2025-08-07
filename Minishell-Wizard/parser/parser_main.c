/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 14:29:31 by fepennar          #+#    #+#             */
/*   Updated: 2025/08/07 15:38:27 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	t_token	*current = tokens;
	
	if (!current)
		return (1);
	
	// Check for leading pipe
	if (current->type == PIPE)
	{
		fprintf(stderr, "bash: syntax error near unexpected token `|'\n");
		return (0);
	}
	
	while (current)
	{
		if (current->type == PIPE)
		{
			// Check for trailing pipe
			if (!current->next)
			{
				fprintf(stderr, "bash: syntax error near unexpected token `|'\n");
				return (0);
			}
			// Check for double pipe
			if (current->next->type == PIPE)
			{
				fprintf(stderr, "bash: syntax error near unexpected token `|'\n");
				return (0);
			}
		}
		
		if (is_redirection(current->type))
		{
			// Check for missing filename
			if (!current->next || current->next->type != WORD)
			{
				fprintf(stderr, "bash: syntax error near unexpected token `newline'\n");
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
	
	// FIXED: Better error handling
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
