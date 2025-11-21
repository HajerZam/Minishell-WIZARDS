/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:10:56 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/29 12:29:24 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * allocates and initializes a new token structure
 * return: Pointer to new token, exits on malloc failure
*/
t_token	*create_token(void)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = NULL;
	token->next = NULL;
	return (token);
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/**
 * removes outer quotes from a string while preserving inner content
 * handles both single and double quotes properly
 * param input: Input string that may contain quotes
 * returns: New string with quotes removed, or NULL on malloc failure
*/

/*
char	*strip_quotes(const char *input)
{
	char	*out;
	size_t	i;
	size_t	j;
	char	quote;

	out = malloc(ft_strlen(input) + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		else
			out[j++] = input[i];
		i++;
	}
	out[j] = '\0';
	return (out);
}
*/

int	has_unclosed_quotes(const char *input)
{
	int		i;
	int		state;

	i = 0;
	state = 0;
	while (input[i])
	{
		if (state == 0)
		{
			if (input[i] == '\'')
				state = 1;
			else if (input[i] == '"')
				state = 2;
		}
		else if (state == 1 && input[i] == '\'')
			state = 0;
		else if (state == 2 && input[i] == '"')
			state = 0;
		i++;
	}
	return (state != 0);
}
