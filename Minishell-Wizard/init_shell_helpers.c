/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 18:23:17 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/15 16:55:50 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_new_multiline_input(char *complete_input, char *line)
{
	char	*new_input;
	size_t	total_len;

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

char	*handle_multiline_input(char *complete_input)
{
	char	*line;

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
	return (create_new_multiline_input(complete_input, line));
}

char	*get_initial_input(t_exec_context *ctx)
{
	char	*line;

	g_signal_received = 0;
	line = readline("WizardShell$ ");
	if (!line)
		return (NULL);
	if (!*line)
	{
		free(line);
		return (ft_strdup(""));
	}
	handle_readline_interruption(ctx);
	return (line);
}
