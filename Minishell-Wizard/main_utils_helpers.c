/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 18:20:43 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 18:29:31 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_input_and_tokens(char *expanded_input,
	t_token *tokens)
{
	free_tokens(tokens);
	free(expanded_input);
}

t_token	*tokenize_and_check_signal(char *expanded_input, t_exec_context *ctx)
{
	t_token	*tokens;

	if (g_signal_received == SIGINT)
	{
		free(expanded_input);
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		return (NULL);
	}
	tokens = tokenize_input(expanded_input);
	if (!tokens)
	{
		free(expanded_input);
		ctx->last_exit_status = 1;
		return (NULL);
	}
	return (tokens);
}

t_cmd	*parse_and_check_signal(t_token *tokens, char *expanded_input,
	t_exec_context *ctx)
{
	t_cmd	*cmd_list;

	if (g_signal_received == SIGINT)
	{
		free_input_and_tokens(expanded_input, tokens);
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		return (NULL);
	}
	cmd_list = parse_command_line(tokens);
	free_input_and_tokens(expanded_input, tokens);
	if (g_signal_received == SIGINT && cmd_list)
	{
		free_cmd_list(cmd_list);
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		return (NULL);
	}
	if (!cmd_list)
	{
		ctx->last_exit_status = 2;
		return (NULL);
	}
	return (cmd_list);
}

int	handle_final_signal_check(t_cmd *cmd_list, t_exec_context *ctx)
{
	if (g_signal_received == SIGINT)
	{
		free_cmd_list(cmd_list);
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		return (1);
	}
	return (0);
}

int	execute_command_or_exit(t_cmd *cmd_list, t_exec_context *ctx)
{
	int	result;

	if (cmd_list && !cmd_list->next && cmd_list->argv && cmd_list->argv[0]
		&& ft_strcmp(cmd_list->argv[0], "exit") == 0)
		return (handle_exit_command(cmd_list, ctx));
	result = execute_pipeline(cmd_list, ctx);
	ctx->last_exit_status = result;
	free_cmd_list(cmd_list);
	cleanup_execution_context(ctx);
	return (1);
}
