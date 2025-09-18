/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:29:52 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/18 20:58:57 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

static void	print_welcome(void)
{
	printf("Welcome to WizardShell!\n");
	printf("Type 'exit' to quit the magical shell.\n");
}

static int	process_input_in_loop(char *input, t_exec_context *ctx)
{
	int	process_result;

	g_signal_received = 0;
	process_result = process_input_line(input, ctx);
	free(input);
	cleanup_backups(ctx);
	if (process_result == 0)
		return (0);
	return (1);
}

static int	get_and_validate_input(t_exec_context *ctx, char **input)
{
	*input = get_complete_input(ctx);
	if (!*input)
	{
		if (ctx->last_exit_status == 130)
			return (-1);
		return (handle_eof_or_signal(ctx));
	}
	if (!**input || ft_strspn(*input, " \t\n") == ft_strlen(*input))
	{
		free(*input);
		return (-1);
	}
	return (1);
}

static int	main_loop(t_env *env, t_exec_context *ctx)
{
	char	*input;
	int		input_result;

	ctx->env = env;
	while (1)
	{
		g_signal_received = 0;
		ctx->in_main_loop = 1;
		init_signals_interactive();
		ctx->in_main_loop = 0;
		input_result = get_and_validate_input(ctx, &input);
		if (input_result == 0)
			return (input_result);
		if (input_result == -1)
			continue ;
		if (process_input_in_loop(input, ctx) == 0)
			break ;
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
	cleanup_resources(&ctx, ctx.env);
	printf("Goodbye from WizardShell!\n");
	return (exit_status);
}
