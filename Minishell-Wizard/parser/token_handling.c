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

t_token	*consume_token(t_parser *parser)
{
	t_token	*token;

	if (!parser->current)
	{
		fprintf(stderr, "No more tokens to consume\n");
		parser->error = 1;
		return (NULL);
	}
	token = parser->current;
	parser->current = parser->current->next;
	return (token);
}
t_token	*peek_token(t_parser *parser)
{
	if (!parser->current)
	{
		fprintf(stderr, "No tokens to peek\n");
		parser->error = 1;
		return (NULL);
	}
	return (parser->current);
}

int	expect_token(t_parser *parser, t_token_type type)
{
	if (!parser->current || parser->current->type != type)
	{
		parser->error = 1;
		return (0);
	}
	return (1);
}
