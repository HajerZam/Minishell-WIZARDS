/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 16:06:39 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 09:42:06 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_input_line(char *input, t_exec_context *ctx)
{
	int	continue_loop;

	if (g_signal_received == SIGINT)
	{
		ctx->last_exit_status = 130;
		g_signal_received = 0;
		return (1);
	}
	if (*input && g_signal_received == 0)
	{
		add_history(input);
		if (g_signal_received == SIGINT)
		{
			ctx->last_exit_status = 130;
			g_signal_received = 0;
			return (1);
		}
		continue_loop = process_command_line(input, ctx);
		if (continue_loop == 0)
			return (0);
	}
	return (1);
}

int	init_shell(t_exec_context *ctx, t_env **env, char **envp)
{
	*env = create_env_copy_direct(envp);
	if (!*env)
	{
		ft_putstr_fd("WizardShell: failed to initialize environment\n", 2);
		return (1);
	}
	if (setup_execution_context(ctx, *env, envp) != 0)
	{
		ft_putstr_fd("WizardShell: failed to initialize execution context\n",
			2);
		free_env(*env);
		return (1);
	}
	return (0);
}

static char	*handle_multiline_input(char *complete_input)
{
	char	*line;
	char	*new_input;
	size_t	total_len;

	line = readline("> ");
	if (!line)
	{
		free(complete_input);
		return (NULL);
	}
	if (g_signal_received == SIGINT)
	{
		free(complete_input);
		free(line);
		return (NULL);
	}
	total_len = strlen(complete_input) + strlen(line) + 2;
	new_input = malloc(total_len);
	if (!new_input)
	{
		free(complete_input);
		free(line);
		return (NULL);
	}
	ft_strlcpy(new_input, complete_input, total_len);
	ft_strlcat(new_input, "\n", total_len);
	ft_strlcat(new_input, line, total_len);
	free(complete_input);
	free(line);
	return (new_input);
}

char	*get_complete_input(void)
{
	char	*line;
	char	*complete_input;

	g_signal_received = 0;
	line = readline("WizardShell$ ");
	if (!line)
		return (NULL);
	if (!*line)
	{
		free(line);
		return (ft_strdup(""));
	}
	complete_input = ft_strdup(line);
	free(line);
	if (!complete_input)
		return (NULL);
	while (has_unclosed_quotes(complete_input))
	{
		if (g_signal_received == SIGINT)
		{
			free(complete_input);
			return (NULL);
		}
		complete_input = handle_multiline_input(complete_input);
		if (!complete_input)
			return (NULL);
	}
	return (complete_input);
}
