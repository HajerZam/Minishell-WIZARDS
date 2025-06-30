/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:34:36 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/30 14:08:44 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_double_quote(t_parse_vars *v)
{
	char	c;

	c = v->input[v->i];
	if (c == '"')
	{
		v->state = STATE_GENERAL;
		v->i++;
	}
	else
	{
		v->buffer[v->j++] = c;
		v->i++;
	}
}
