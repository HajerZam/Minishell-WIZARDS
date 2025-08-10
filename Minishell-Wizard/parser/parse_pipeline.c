/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:23:20 by fepennar          #+#    #+#             */
/*   Updated: 2025/08/07 15:38:40 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*finalizes the command, function originally from parse_command.c*/

void	finalize_command(t_cmd *cmd)
{
	if (!cmd || !cmd->argv)
		return ;
	cmd->is_builtin = is_builtin_command(cmd->argv);
}

int	check_pipeline(t_parser *parser, t_cmd *current_cmd, t_cmd *first_cmd)
{
	while (parser->current && parser->current->type == PIPE)
	{
		consume_token(parser);
		if (!parser->current || parser->current->type == PIPE)
		{
			fprintf(stderr, "bash: syntax error near unexpected token `|'\n");
			parser->error = 1;
			free_cmd_list(first_cmd);
			return (0);
		}
		current_cmd->next = parse_command(parser);
		if (!current_cmd->next || parser->error)
		{
			fprintf(stderr, "bash: syntax error in pipeline\n");
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
	if (parser->current->type == PIPE)
	{
		fprintf(stderr, "bash: syntax error near unexpected token `|'\n");
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
