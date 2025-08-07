/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fepennar <fepennar@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:23:20 by fepennar          #+#    #+#             */
/*   Updated: 2025/07/26 19:23:31 by fepennar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipeline(t_parser *parser, t_cmd *current_cmd, t_cmd *first_cmd)
{
	while (parser->current && parser->current->type == PIPE)
	{
		consume_token(parser); // Consume the PIPE token
		if (!parser->current)
		{
			fprintf(stderr, "Unexpected end of input after pipe\n");
			parser->error = 1;
			free_cmd_list(first_cmd);
			return (0);
		}
		current_cmd->next = parse_command(parser);
		if (!current_cmd->next || parser->error)
		{
			fprintf(stderr, "Error parsing command after pipe\n");
			free_cmd_list(first_cmd);
			return (0);
		}
		current_cmd = current_cmd->next;
	}
	return (1);
}

t_cmd	*parse_pipeline(t_parser *parser)
{
	t_cmd	*first_cmd;
	t_cmd	*current_cmd;

	if (!parser->current)
	{
		fprintf(stderr, "No tokens to parse\n");
		parser->error = 1;
		return (NULL);
	}
	first_cmd = parse_command(parser);
	if (!first_cmd || parser->error)
		return (first_cmd);
	current_cmd = first_cmd;
	if (check_pipeline(parser, current_cmd, first_cmd) == 0)
		return (NULL);
	return (first_cmd);
}
