/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:27:08 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/25 14:27:08 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_general_state(t_parse_vars *v)
{
	char	c;

	c = v->input[v->i];
	if (c == '\'')
		v->state = STATE_IN_SINGLE_QUOTE;
	else if (c == '"')
		v->state = STATE_IN_DOUBLE_QUOTE;
	else if (isspace(c) || is_operator(c))
		return ;
	else if (c == '\\' && v->input[v->i + 1])
	{
		v->i++;
		v->buffer[v->j++] = v->input[v->i++];
		return ;
	}
	else
		v->buffer[v->j++] = c;
	v->i++;
}

static void	handle_single_quote(t_parse_vars *v)
{
	char	c;

	c = v->input[v->i];
	if (c == '\'')
		v->state = STATE_GENERAL;
	else
		v->buffer[v->j++] = c;
	v->i++;
}

t_token	*parse_word(const char **ptr)
{
	t_parse_vars	v;

	v.input = *ptr;
	v.token = create_token();
	v.buffer = malloc(strlen(v.input) + 1);
	if (!v.buffer)
		return (NULL);
	v.i = 0;
	v.j = 0;
	v.state = STATE_GENERAL;
	while (v.input[v.i])
	{
		if (v.state == STATE_GENERAL)
			handle_general_state(&v);
		else if (v.state == STATE_IN_SINGLE_QUOTE)
			handle_single_quote(&v);
		else if (v.state == STATE_IN_DOUBLE_QUOTE)
			handle_double_quote(&v);
	}
	v.buffer[v.j] = '\0';
	v.token->type = WORD;
	v.token->value = strip_quotes(v.buffer);
	free(v.buffer);
	*ptr += v.i;
	return (v.token);
}
