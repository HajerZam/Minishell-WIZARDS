/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_checks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 19:48:46 by halzamma          #+#    #+#             */
/*   Updated: 2026/01/07 15:39:18 by halzamma         ###   ########.fr       */
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
	int	exit_code;

	if (!command_path)
	{
		if (!ft_strchr(cmd->argv[0], '/'))
		{
			print_execution_error(cmd->argv[0], "command not found");
			exit_code = 127;
		}
		else
		{
			if (is_directory(cmd->argv[0]))
				exit_code = 126;
			else
				exit_code = 127;
		}
		cleanup_child_process(ctx, cmd_list);
		exit(exit_code);
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
