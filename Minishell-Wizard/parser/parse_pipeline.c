/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:23:20 by fepennar          #+#    #+#             */
/*   Updated: 2025/08/29 13:12:57 by halzamma         ###   ########.fr       */
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
			ft_putstr_fd("wizardshell: syntax error unexpected token `|'\n", 2);
			parser->error = 1;
			free_cmd_list(first_cmd);
			return (0);
		}
		current_cmd->next = parse_command(parser);
		if (!current_cmd->next || parser->error)
		{
			ft_putstr_fd("wizardshell: syntax error unexpected token `|'\n", 2);
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
		ft_putstr_fd("wizardshell: syntax error: empty command\n", 2);
		parser->error = 1;
		return (NULL);
	}
	if (parser->current->type == PIPE)
	{
		ft_putstr_fd("wizardshell: syntax error unexpected token `|'\n", 2);
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
