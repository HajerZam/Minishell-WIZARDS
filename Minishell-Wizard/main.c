/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:29:52 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/11 15:33:05 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

static void	print_welcome(void)
{
	printf("hellooo to Minishell Wizard!\n");
	printf("Type 'exit' to quit.\n");
}
void	cleanup_and_exit(t_exec_context *ctx, t_env *env, int exit_status)
{
	if (ctx)
		cleanup_execution_context(ctx);
	if (env)
		free_env(env);
	printf("Exiting Minishell Wizard with status %d\n", exit_status);
	exit(exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_exec_context	ctx;
	t_env			*env;
	t_token			*tokens;
	t_cmd			*cmd_list;
	int				should_continue;

	(void)argc;
	(void)argv;
	print_welcome();

	env = init_env(envp);
	if (!env)
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		return (1);
	}
	if (init_execution_context(&ctx, envp) != 0)
	{
		ft_putstr_fd("minishell: failed to initialize execution context\n", 2);
		free_env(env);
		return (1);
	}

	init_signals();
	should_continue = 1;

	while (should_continue)
	{
		g_signal_received = 0;
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		if (*input)
			add_history(input);
		if (g_signal_received == SIGINT)
		{
			ctx.last_exit_status = 130;
			free(input);
			continue;
		}
		if (*input)
		{
			tokens = tokenize_input(input);
			if (tokens)
			{
				cmd_list = parse_command_line(tokens);
				free_tokens(tokens);
				if (cmd_list)
				{
					execute_pipeline(cmd_list, &ctx);
					free_cmd_list(cmd_list);
				}
				else
					ft_putstr_fd("minishell: syntax error\n", 2);
			}
		}
		free(input);
	}
	cleanup_and_exit(&ctx, env, ctx.last_exit_status);
	return (0);
}
