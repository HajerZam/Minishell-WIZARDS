/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 16:06:39 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/27 10:16:42 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_input_line(char *input, t_exec_context *ctx)
{
	int	continue_loop;

	if (*input && g_signal_received == 0)
	{
		add_history(input);
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
	char	*temp;

	line = readline("> ");
	if (!line)
	{
		free(complete_input);
		return (NULL);
	}
	temp = complete_input;
	complete_input = malloc(strlen(temp) + strlen(line) + 2);
	if (!complete_input)
	{
		free(temp);
		free(line);
		return (NULL);
	}
	sprintf(complete_input, "%s\n%s", temp, line);
	free(temp);
	free(line);
	return (complete_input);
}

char	*get_complete_input(void)
{
	char	*line;
	char	*complete_input;

	line = readline("WizardShell$ ");
	if (!line)
		return (NULL);
	complete_input = ft_strdup(line);
	free(line);
	while (has_unclosed_quotes(complete_input))
	{
		complete_input = handle_multiline_input(complete_input);
		if (!complete_input)
			return (NULL);
	}
	return (complete_input);
}
