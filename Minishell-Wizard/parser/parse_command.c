/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:59:45 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/07 15:38:47 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	finalize_command(t_cmd *cmd)
{
	if (!cmd || !cmd->argv)
		return ;
	cmd->is_builtin = is_builtin_command(cmd->argv);
}

int	validate_parsed_command(t_cmd *cmd, t_parser *parser)
{
	// FIXED: Correct logic for checking redirections without commands
	if ((!cmd->argv || !cmd->argv[0]) && 
		(cmd->input_fd != 0 || cmd->output_fd != 1 || cmd->is_heredoc))
	{
		fprintf(stderr, "bash: syntax error near unexpected token\n");
		parser->error = 1;
		return (0);
	}
	
	// Reject completely empty commands
	if (!cmd->argv || !cmd->argv[0])
	{
		fprintf(stderr, "bash: command not found\n");
		parser->error = 1;
		return (0);
	}
	
	if (ft_strlen(cmd->argv[0]) == 0)
	{
		fprintf(stderr, "bash: command not found\n");
		parser->error = 1;
		return (0);
	}
	
	return (1);
}

// FIXED: Removed double token consumption
int	pce_error_check(t_parser *parser, t_cmd *cmd, int arg_count, int arg_index)
{
	while (parser->current && parser->current->type != PIPE)
	{
		if (parser->current->type == WORD)
		{
			if (handle_word_token(cmd, parser, &arg_index, arg_count) == 0)
				return (0);
			// Token is consumed in handle_word_token, don't advance here
		}
		else if (is_redirection(parser->current->type))
		{
			if (parse_redirections(parser, cmd) == 0)
				return (0);
			// Token is consumed in parse_redirections, don't advance here
		}
		else
		{
			fprintf(stderr, "bash: syntax error near unexpected token\n");
			parser->error = 1;
			return (0);
		}
		// REMOVED: parser->current = parser->current->next; 
		// Functions above handle token consumption
	}
	return (1);
}

int	parse_command_elements(t_parser *parser, t_cmd *cmd)
{
	int	arg_count;
	int	arg_index;

	arg_count = count_words_until_pipe(parser);
	if (arg_count > 0)
	{
		cmd->argv = allocate_argv(arg_count);
		if (!cmd->argv)
		{
			parser->error = 1;
			return (0);
		}
	}
	arg_index = 0;
	if (pce_error_check(parser, cmd, arg_count, arg_index) == 0)
		return (0);
	return (1);
}

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
	{
		perror("Failed to allocate memory for command");
		return (NULL);
	}
	cmd->argv = NULL;
	cmd->input_fd = 0;
	cmd->output_fd = 1;
	cmd->next = NULL;
	cmd->tokens = NULL;
	cmd->heredoc_fd = -1;
	cmd->is_heredoc = 0;
	cmd->is_builtin = 0;
	cmd->envp = NULL;
	cmd->exit_status = 0;
	return (cmd);
}

t_cmd	*parse_command(t_parser *parser)
{
	t_cmd	*cmd;

	if (!parser->current)
		return (NULL);
	cmd = create_cmd();
	if (!cmd)
	{
		parser->error = 1;
		return (NULL);
	}
	if (!parse_command_elements(parser, cmd))
	{
		free_cmd(cmd);
		return (NULL);
	}
	if (!validate_parsed_command(cmd, parser))
	{
		free_cmd(cmd);
		return (NULL);
	}
	finalize_command(cmd);
	return (cmd);
}
