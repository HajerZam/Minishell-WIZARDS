/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:06:03 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/14 14:24:33 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int execute_builtin(t_cmd *cmd, t_env *env)
{
    if (strcmp(cmd->argv[0], "cd") == 0)
        return (builtin_cd(cmd->argv, env));
    else if (strcmp(cmd->argv[0], "echo") == 0)
        return (builtin_echo(cmd->argv));
    else if (strcmp(cmd->argv[0], "pwd") == 0)
        return (builtin_pwd());
    else if (strcmp(cmd->argv[0], "exit") == 0)
        return (builtin_exit(cmd->argv));
    else if (strcmp(cmd->argv[0], "env") == 0)
        return (builtin_env(env));
    else if (strcmp(cmd->argv[0], "export") == 0)
        return (builtin_export(cmd->argv, env));
    else if (strcmp(cmd->argv[0], "unset") == 0)
        return (builtin_unset(cmd->argv, env));
    return (127); // Command not found
}

int is_builtin(const char *cmd)
{
    if (strcmp(cmd, "cd") == 0 || strcmp(cmd, "echo") == 0 ||
        strcmp(cmd, "pwd") == 0 || strcmp(cmd, "exit") == 0 ||
        strcmp(cmd, "env") == 0 || strcmp(cmd, "export") == 0 ||
        strcmp(cmd, "unset") == 0)
        return (1);
    return (0);
}

int handle_builtin(t_cmd *cmd, t_env *env)
{
    int exit_status;
    int saved_stdin;
    int saved_stdout;
    
    if (!cmd || !cmd->argv || !cmd->argv[0] || !env)
        return (1);
    if (!is_builtin(cmd->argv[0]))
        return (127);
    if (!setup_builtin_fds(cmd, &saved_stdin, &saved_stdout))
    {
        perror("minishell: setup_builtin_fds failed");
        return (1);
    }
    exit_status = execute_builtin(cmd, env);
    restore_builtin_fds(saved_stdin, saved_stdout);
    return (exit_status);
}

