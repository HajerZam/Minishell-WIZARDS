/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_checks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 19:48:46 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 19:48:48 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_command_path(char *command)
{
	char	**paths;
	char	*path_env;

	if (!command)
		return (NULL);
	if (ft_strchr(command, '/'))
		return (handle_absolute_path(command));
	path_env = getenv("PATH");
	if (!path_env)
	{
		print_execution_error(command, "No such file or directory");
		return (NULL);
	}
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (search_in_path_dirs(paths, command));
}

void	check_cmd(t_cmd *cmd, t_exec_context *ctx, t_cmd *cmd_list,
		char *command_path)
{
	if (!command_path)
	{
		print_execution_error(cmd->argv[0], "command not found");
		cleanup_child_process(ctx, cmd_list);
		exit(127);
	}
}

void	check_envp(t_exec_context *ctx, t_cmd *cmd_list, char *command_path,
		char **current_envp)
{
	if (!current_envp)
	{
		free(command_path);
		cleanup_child_process(ctx, cmd_list);
		exit(1);
	}
}

void	free_ctx_list(t_exec_context *ctx, t_cmd *cmd_list)
{
	if (ctx->pids)
		free(ctx->pids);
	if (ctx->env)
		free_env(ctx->env);
	if (cmd_list)
		free_cmd_list(cmd_list);
}
