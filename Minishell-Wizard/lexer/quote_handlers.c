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
		// Inside double quotes: add everything literally except we'll handle $ later
		// For now, just adding the character
		v->buffer[v->j++] = c;
		v->i++;
	}
}