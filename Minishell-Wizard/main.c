/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:29:52 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/17 16:26:32 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

static void	print_welcome(void)
{
	printf("🧙‍♂️ Welcome to Minishell Wizard!\n");
	printf("Type 'exit' to quit the magical shell.\n");
}

/**
 * cleanup_and_exit - Clean up all resources and exit
 * ctx: Execution context to clean up
 * env: Environment list to free  
 * exit_status: Exit code to use
 */
void	cleanup_and_exit(t_exec_context *ctx, t_env *env, int exit_status)
{
	printf("DEBUG: Starting cleanup and exit with status %d\n", exit_status);
	
	if (ctx)
	{
		printf("DEBUG: Cleaning up execution context\n");
		cleanup_execution_context(ctx);
	}
	if (env)
	{
		printf("DEBUG: Freeing environment\n");
		free_env(env);
	}
	
	printf("DEBUG: Clearing readline history\n");
	rl_clear_history();
	
	printf("✨ Goodbye from Minishell Wizard!\n");
	exit(exit_status);
}

/**
 * process_command_line - Process a single command line input
 * @input: The input string to process
 * @env: Environment variables
 * @ctx: Execution context
 * Return: 1 to continue, 0 to exit
 */
static int	process_command_line(char *input, t_env *env, t_exec_context *ctx)
{
	t_token	*tokens;
	t_cmd	*cmd_list;
	char	*expanded_input;

	printf("DEBUG: Processing command line: '%s'\n", input);
	
	if (!input || !*input)
		return (1);

	// Expand variables first
	printf("DEBUG: Expanding variables\n");
	expanded_input = expand_variables(input, env, ctx->last_exit_status);
	if (!expanded_input)
	{
		ft_putstr_fd("minishell: expansion error\n", 2);
		ctx->last_exit_status = 1;
		return (1);
	}
	printf("DEBUG: After expansion: '%s'\n", expanded_input);

	// Tokenize the expanded input
	printf("DEBUG: Tokenizing input\n");
	tokens = tokenize_input(expanded_input);
	free(expanded_input);
	if (!tokens)
	{
		printf("DEBUG: Tokenization failed\n");
		ctx->last_exit_status = 1;
		return (1);
	}

	// Parse tokens into command structure
	printf("DEBUG: Parsing command line\n");
	cmd_list = parse_command_line(tokens);
	free_tokens(tokens);
	if (!cmd_list)
	{
		printf("DEBUG: Parsing failed\n");
		ctx->last_exit_status = 2;
		return (1);
	}

	// Update env pointer in context
	ctx->env = env;

	// Handle special builtins that must run in main process (exit only)
	if (cmd_list && !cmd_list->next && cmd_list->argv && cmd_list->argv[0])
	{
		printf("DEBUG: Checking for exit command: '%s'\n", cmd_list->argv[0]);
		if (ft_strcmp(cmd_list->argv[0], "exit") == 0)
		{
			int exit_status = builtin_exit(cmd_list->argv);
			free_cmd_list(cmd_list);
			cleanup_and_exit(ctx, env, exit_status);
			// This line won't execute
		}
	}

	// Execute the pipeline
	printf("DEBUG: Executing pipeline\n");
	ctx->last_exit_status = execute_pipeline(cmd_list, ctx);
	printf("DEBUG: Pipeline executed, exit status: %d\n", ctx->last_exit_status);
	
	printf("DEBUG: Freeing command list\n");
	free_cmd_list(cmd_list);
	printf("DEBUG: Command processing completed\n");
	
	return (1); // Always continue unless explicit exit
}

/**
 * handle_readline_interruption - Handle signal interruption during readline
 * ctx: Execution context to update
 */
static void	handle_readline_interruption(t_exec_context *ctx)
{
	if (g_signal_received == SIGINT)
	{
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		printf("\n");
	}
}

/**
 * setup_execution_context - Initialize execution context with environment
 * ctx: Context to initialize
 * env: Environment list
 * envp: Environment array
 * Return: 0 on success, 1 on error
 */
static int	setup_execution_context(t_exec_context *ctx, t_env *env, char **envp)
{
	if (init_execution_context(ctx, envp) != 0)
		return (1);
		
	ctx->env = env;
	return (0);
}

/**
 * main_loop - Main interactive loop
 * env: Environment variables
 * ctx: Execution context
 * Return: Exit status
 */
static int	main_loop(t_env *env, t_exec_context *ctx)
{
	char	*input;
	int		should_continue;

	should_continue = 1;
	while (should_continue)
	{
		// Reset signal flag
		g_signal_received = 0;
		
		// Read input with prompt
		input = readline("minishell🪄 $ ");
		
		// Handle EOF (Ctrl-D)
		if (!input)
		{
			printf("exit\n");
			break;
		}

		// Check for signal interruption after readline
		handle_readline_interruption(ctx);

		// Add to history if not empty and no signal received
		if (*input && g_signal_received == 0)
		{
			add_history(input);
			should_continue = process_command_line(input, env, ctx);
		}

		free(input);
		printf("DEBUG: Loop iteration completed\n");
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

	printf("DEBUG: Starting minishell\n");

	// Initialize environment from envp
	env = init_env(envp);
	if (!env)
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		return (1);
	}
	printf("DEBUG: Environment initialized\n");

	// Initialize execution context
	if (setup_execution_context(&ctx, env, envp) != 0)
	{
		ft_putstr_fd("minishell: failed to initialize execution context\n", 2);
		free_env(env);
		return (1);
	}
	printf("DEBUG: Execution context initialized\n");

	// Setup signal handling
	init_signals();
	printf("DEBUG: Signals initialized\n");
	
	// Display welcome message
	print_welcome();

	// Enter main interactive loop
	printf("DEBUG: Entering main loop\n");
	exit_status = main_loop(env, &ctx);

	// Clean up and exit
	printf("DEBUG: Exiting main loop\n");
	cleanup_and_exit(&ctx, env, exit_status);
	
	return (0); // This line won't execute, but for safety
}
