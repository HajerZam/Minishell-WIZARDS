/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:10:56 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/30 14:13:41 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
