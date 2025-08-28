/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 15:59:24 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/28 21:34:08 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_resources(t_exec_context *ctx, t_env *env)
{
	if (ctx)
    {
        cleanup_execution_context(ctx);
        if (ctx->stdin_backup != -1)
        {
            dup2(ctx->stdin_backup, STDIN_FILENO);
            close(ctx->stdin_backup);
        }
        if (ctx->stdout_backup != -1)
        {
            dup2(ctx->stdout_backup, STDOUT_FILENO);
            close(ctx->stdout_backup);
        }
    }
    if (env)
        free_env(env);
    clear_history();
}

void	cleanup_and_exit(t_exec_context *ctx, t_env *env, int exit_status)
{
	if (ctx)
		cleanup_execution_context(ctx);
	if (env)
		free_env(env);
	rl_clear_history();
	printf("Goodbye from WizardShell!\n");
	exit(exit_status);
}

int	handle_exit_command(t_cmd *cmd_list, t_exec_context *ctx)
{
	int		exit_status;
	char	*endptr;
	long	val;

	printf("exit\n");
	exit_status = 0;
	if (cmd_list->argv[1])
	{
		val = strtol(cmd_list->argv[1], &endptr, 10);
		if (*endptr != '\0')
		{
			fprintf(stderr, "bash: exit: %s: numeric argument required\n",
				cmd_list->argv[1]);
			exit_status = 2;
		}
		else
			exit_status = (int)val;
	}
	free_cmd_list(cmd_list);
	ctx->last_exit_status = exit_status;
	return (0);
}

int	process_tokens(char *expanded_input, t_exec_context *ctx)
{
    t_token *tokens;
    t_cmd   *cmd_list;

    tokens = tokenize_input(expanded_input);
    if (!tokens)
    {
        free(expanded_input);
        ctx->last_exit_status = 1;
        return (1);
    }
    cmd_list = parse_command_line(tokens);
    free_tokens(tokens);
    free(expanded_input);
    if (!cmd_list)
    {
        ctx->last_exit_status = 2;
        return (1);
    }
    if (cmd_list && !cmd_list->next && cmd_list->argv && cmd_list->argv[0]
        && ft_strcmp(cmd_list->argv[0], "exit") == 0)
        return (handle_exit_command(cmd_list, ctx));
    ctx->last_exit_status = execute_pipeline(cmd_list, ctx);
    free_cmd_list(cmd_list);
    return (1);
}

void	handle_readline_interruption(t_exec_context *ctx)
{
	if (g_signal_received == SIGINT)
	{
		ctx->last_exit_status = 130;
		g_signal_received = 0;
	}
}
