/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:34:36 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/25 16:34:36 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_escaped_double_quote(t_parse_vars *v)
{
	int		escape_count;
	int		k;
	char	next;

	escape_count = 0;
	while (v->input[v->i] == '\\')
	{
		escape_count++;
		v->i++;
	}
	next = v->input[v->i];
	if (next == '"')
	{
		k = 0;
		while (k++ < escape_count / 2)
			v->buffer[v->j++] = '\\';
		if (escape_count % 2 == 1)
			v->buffer[v->j++] = v->input[v->i++];
	}
	else
	{
		k = 0;
		while (k++ < escape_count)
			v->buffer[v->j++] = '\\';
	}
}

static void	handle_non_escaped_double_quote(t_parse_vars *v)
{
	if (v->input[v->i] == '"')
	{
		v->state = STATE_GENERAL;
		v->i++;
	}
	else
		v->buffer[v->j++] = v->input[v->i++];
}

void	handle_double_quote(t_parse_vars *v)
{
	if (v->input[v->i] == '\\')
		handle_escaped_double_quote(v);
	else
		handle_non_escaped_double_quote(v);
}
