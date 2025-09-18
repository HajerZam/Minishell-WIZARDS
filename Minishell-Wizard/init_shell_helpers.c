/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 18:23:17 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/18 19:31:12 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	get_unclosed_quote_type(char *input)
{
	int	in_single_quote;
	int	in_double_quote;
	int	i;

	in_single_quote = 0;
	in_double_quote = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (input[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	if (in_single_quote)
		return ('\'');
	if (in_double_quote)
		return ('"');
	return (0);
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
