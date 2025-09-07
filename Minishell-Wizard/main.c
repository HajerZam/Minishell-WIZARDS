/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:29:52 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 09:24:38 by halzamma         ###   ########.fr       */
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
	int		result;

	if (!input || !*input)
		return (1);
		
	/* Check for signal before processing */
	if (g_signal_received == SIGINT)
	{
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		return (1);
	}
	init_signals_execution();
	expanded_input = expand_variables(input, ctx->env, ctx->last_exit_status);
	if (!expanded_input)
	{
		ft_putstr_fd("wizardshell: expansion error\n", 2);
		ctx->last_exit_status = 1;
		init_signals_interactive();
		return (1);
	}
	if (g_signal_received == SIGINT)
	{
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		free(expanded_input);
		init_signals_interactive();
		return (1);
	}
	result = process_tokens(expanded_input, ctx);
	init_signals_interactive();
	return (result);
}

int	setup_execution_context(t_exec_context *ctx, t_env *env,
	char **envp)
{
	if (init_execution_context(ctx, envp) != 0)
		return (1);
	ctx->env = env;
	return (0);
}

static int	handle_eof_or_signal(t_exec_context *ctx)
{
	/* Handle EOF (Ctrl-D) */
	(void)ctx;  /* Suppress unused parameter warning */
	
	if (!isatty(STDIN_FILENO))
	{
		/* If input is not from terminal, EOF means end of input */
		printf("exit\n");
		return (0);
	}
	
	/* Handle Ctrl-D in interactive mode */
	printf("exit\n");
	return (0);
}

/* Replace your main_loop function with this final fixed version */

static int	main_loop(t_env *env, t_exec_context *ctx)
{
	char	*input;
	int		process_result;

	ctx->env = env;
	while (1)
	{
		/* Reset signal flag before each iteration */
		g_signal_received = 0;
		ctx->in_main_loop = 1;
		init_signals_interactive();
		
		input = get_complete_input();
		ctx->in_main_loop = 0;
		
		/* Handle EOF (Ctrl+D) */
		if (!input)
		{
			return (handle_eof_or_signal(ctx));
		}
		
		/* Skip empty input or whitespace-only input */
		if (!*input || strspn(input, " \t\n") == strlen(input))
		{
			free(input);
			continue;
		}
		
		/* CRITICAL FIX: Clear signal flag right before processing */
		g_signal_received = 0;
		
		/* Process the command */
		process_result = process_input_line(input, ctx);
		free(input);
		
		/* Reset any fd backups after each command */
		if (ctx->stdin_backup != -1)
		{
			close(ctx->stdin_backup);
			ctx->stdin_backup = -1;
		}
		if (ctx->stdout_backup != -1)
		{
			close(ctx->stdout_backup);
			ctx->stdout_backup = -1;
		}
		
		if (process_result == 0)
			break;
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
	init_exec_context_struct(&ctx);
	if (init_shell(&ctx, &env, envp) != 0)
		return (1);
	init_signals_interactive();
	print_welcome();
	exit_status = main_loop(env, &ctx);
	cleanup_resources(&ctx, env);
	printf("Goodbye from WizardShell!\n");
	return (exit_status);
}
