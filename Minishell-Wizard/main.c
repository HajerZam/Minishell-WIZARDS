/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:29:52 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/27 08:50:49 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

static void	print_welcome(void)
{
	printf("Welcome to WizardShell!\n");
	printf("Type 'exit' to quit the magical shell.\n");
}

int	process_command_line(char *input, t_exec_context *ctx)
{
	char	*expanded_input;

	if (!input || !*input)
		return (1);
	expanded_input = expand_variables(input, ctx->env, ctx->last_exit_status);
	if (!expanded_input)
	{
		ft_putstr_fd("minishell: expansion error\n", 2);
		ctx->last_exit_status = 1;
		return (1);
	}
	return (process_tokens(expanded_input, ctx));
}

int	setup_execution_context(t_exec_context *ctx, t_env *env,
	char **envp)
{
	if (init_execution_context(ctx, envp) != 0)
		return (1);
	ctx->env = env;
	return (0);
}

static int	main_loop(t_env *env, t_exec_context *ctx)
{
	char	*input;

	ctx->env = env;
	while (1)
	{
		g_signal_received = 0;
		input = get_complete_input();
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		handle_readline_interruption(ctx);
		if (process_input_line(input, ctx) == 0)
		{
			free(input);
			break ;
		}
		free(input);
	}
	return (ctx->last_exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_exec_context	ctx;
	t_env			*env;
	int				exit_status;

	(void)argc;
	(void)argv;
	if (init_shell(&ctx, &env, envp) != 0)
		return (1);
	init_signals();
	print_welcome();
	exit_status = main_loop(env, &ctx);
	cleanup_resources(&ctx, env);
	printf("Goodbye from WizardShell!\n");
	return (exit_status);
}
