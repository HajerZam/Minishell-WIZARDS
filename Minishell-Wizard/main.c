/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:29:52 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/15 21:46:03 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

static void	print_welcome(void)
{
	printf("Welcome to Minishell Wizard!\n");
	printf("Type 'exit' to quit.\n");
}

void	cleanup_and_exit(t_exec_context *ctx, t_env *env, int exit_status)
{
	if (ctx)
	{
		cleanup_execution_context(ctx);
		restore_std_fds(ctx);
	}
	if (env)
		free_env(env);
	rl_clear_history();
	printf("Goodbye!\n");
	exit(exit_status);
}

static int	handle_special_builtins(t_cmd *cmd, t_env *env, t_exec_context *ctx)
{
	int	status;

	// Only handle exit in main process (other builtins handled in executor)
	if (cmd->argv && cmd->argv[0] && ft_strcmp(cmd->argv[0], "exit") == 0)
	{
		status = builtin_exit(cmd->argv);
		cleanup_and_exit(ctx, env, status);
	}
	return (0);
}

static int	process_command_line(char *input, t_env *env, t_exec_context *ctx)
{
	t_token	*tokens;
	t_cmd	*cmd_list;
	char	*expanded_input;

	if (!input || !*input)
		return (1);

	// Expand variables first
	expanded_input = expand_variables(input, env, ctx->last_exit_status);
	if (!expanded_input)
	{
		ft_putstr_fd("minishell: expansion error\n", 2);
		ctx->last_exit_status = 1;
		return (1);
	}
	
	printf("DEBUG: After expansion: '%s'\n", expanded_input);

	// Tokenize
	tokens = tokenize_input(expanded_input);
	free(expanded_input);
	if (!tokens)
		return (1);

	// Parse
	cmd_list = parse_command_line(tokens);
	free_tokens(tokens);
	if (!cmd_list)
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		ctx->last_exit_status = 2;
		return (1);
	}

	// Handle special builtins that must run in main process
	if (cmd_list && !cmd_list->next)
		handle_special_builtins(cmd_list, env, ctx);

	// Execute the pipeline
	execute_pipeline(cmd_list, ctx);
	free_cmd_list(cmd_list);
	
	return (1);
}

static void	handle_readline_interruption(t_exec_context *ctx)
{
	if (g_signal_received == SIGINT)
	{
		ctx->last_exit_status = 130;
		g_signal_received = 0;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_exec_context	ctx;
	t_env			*env;
	int				should_continue;

	(void)argc;
	(void)argv;

	// Initialize environment
	env = init_env(envp);
	if (!env)
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		return (1);
	}

	// Initialize execution context
	if (init_execution_context(&ctx, envp) != 0)
	{
		ft_putstr_fd("minishell: failed to initialize execution context\n", 2);
		free_env(env);
		return (1);
	}

	// Setup signals and display welcome
	init_signals();
	print_welcome();

	should_continue = 1;
	while (should_continue)
	{
		// Reset signal
		g_signal_received = 0;
		
		// Read input
		input = readline("minishell$ ");
		
		// Debug: print what we read
		if (input)
			printf("DEBUG: Read input: '%s'\n", input);
		else
			printf("DEBUG: Got NULL input (EOF)\n");
		
		// Handle EOF (Ctrl-D)
		if (!input)
		{
			printf("exit\n");
			break;
		}

		// Check for signal interruption
		handle_readline_interruption(&ctx);

		// Add to history if not empty
		if (*input)
			add_history(input);

		// Process the command if not empty and no signal
		if (*input && g_signal_received == 0)
			process_command_line(input, env, &ctx);

		free(input);
	}

	cleanup_and_exit(&ctx, env, ctx.last_exit_status);
	return (0);
}
