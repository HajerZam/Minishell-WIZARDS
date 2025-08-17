/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:29:52 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/17 22:27:33 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

static void print_welcome(void)
{
    printf("🧙‍♂️ Welcome to Minishell Wizard!\n");
    printf("Type 'exit' to quit the magical shell.\n");
}

void cleanup_and_exit(t_exec_context *ctx, t_env *env, int exit_status)
{
    if (ctx)
        cleanup_execution_context(ctx);
    if (env)
        free_env(env);
    rl_clear_history();
    printf("✨ Goodbye from Minishell Wizard!\n");
    exit(exit_status);
}

/* FIXED: Only exit on actual exit command, not on other builtins */
static int process_command_line(char *input, t_env *env, t_exec_context *ctx)
{
    t_token *tokens;
    t_cmd *cmd_list;
    char *expanded_input;

    if (!input || !*input)
        return 1;

    expanded_input = expand_variables(input, env, ctx->last_exit_status);
    if (!expanded_input)
    {
        ft_putstr_fd("minishell: expansion error\n", 2);
        ctx->last_exit_status = 1;
        return 1; // Continue running
    }

    tokens = tokenize_input(expanded_input);
    free(expanded_input);
    if (!tokens)
    {
        ctx->last_exit_status = 1;
        return 1; // Continue running
    }

    cmd_list = parse_command_line(tokens);
    free_tokens(tokens);
    if (!cmd_list)
    {
        ctx->last_exit_status = 2;
        return 1; // Continue running
    }

    ctx->env = env;

    /* CRITICAL FIX: Only handle exit command specially, let other builtins run normally */
    if (cmd_list && !cmd_list->next && cmd_list->argv && cmd_list->argv[0])
    {
        if (ft_strcmp(cmd_list->argv[0], "exit") == 0)
        {
            printf("exit\n");
            int exit_status = 0;
            if (cmd_list->argv[1])
            {
                char *endptr;
                long val = strtol(cmd_list->argv[1], &endptr, 10);
                if (*endptr != '\0')
                {
                    fprintf(stderr, "bash: exit: %s: numeric argument required\n", cmd_list->argv[1]);
                    exit_status = 2;
                }
                else
                    exit_status = (int)val;
            }
            free_cmd_list(cmd_list);
            cleanup_and_exit(ctx, env, exit_status);
        }
    }
    ctx->last_exit_status = execute_pipeline(cmd_list, ctx);
    free_cmd_list(cmd_list);
    return 1; /* ALWAYS return 1 to continue shell loop */
}

static void handle_readline_interruption(t_exec_context *ctx)
{
    if (g_signal_received == SIGINT)
    {
        ctx->last_exit_status = 130;
        g_signal_received = 0;
    }
}

static int setup_execution_context(t_exec_context *ctx, t_env *env, char **envp)
{
    if (init_execution_context(ctx, envp) != 0)
        return 1;
    ctx->env = env;
    return 0;
}

static int main_loop(t_env *env, t_exec_context *ctx)
{
    char *input;

    while (1)
    {
        g_signal_received = 0;
        input = readline("minishell🪄 $ ");
        if (!input)
        {
            printf("exit\n");
            break;
        }
        handle_readline_interruption(ctx);
        if (*input && g_signal_received == 0)
        {
            add_history(input);
            process_command_line(input, env, ctx); /* This should not exit unless "exit" command */
        }
        free(input);
    }
    return ctx->last_exit_status;
}

int main(int argc, char **argv, char **envp)
{
    t_exec_context ctx;
    t_env *env;
    int exit_status;

    (void)argc;
    (void)argv;
    env = init_env(envp);
    if (!env)
    {
        ft_putstr_fd("minishell: failed to initialize environment\n", 2);
        return (1);
    }
    if (setup_execution_context(&ctx, env, envp) != 0)
    {
        ft_putstr_fd("minishell: failed to initialize execution context\n", 2);
        free_env(env);
        return (1);
    }
    init_signals();
    print_welcome();
    exit_status = main_loop(env, &ctx);
    cleanup_and_exit(&ctx, env, exit_status);
    return (0); /* This will never be reached, cleanup_and_exit exits */
}
