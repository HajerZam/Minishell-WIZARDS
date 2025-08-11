/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:10:56 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/07 16:55:27 by halzamma         ###   ########.fr       */
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
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
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
