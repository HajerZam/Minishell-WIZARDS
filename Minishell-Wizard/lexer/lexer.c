/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:27:41 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/29 12:52:09 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

static t_token	*parse_next_token(const char **ptr)
{
	t_token	*new_token;

	if (is_operator(**ptr))
		new_token = parse_operator(ptr);
	else
		new_token = parse_word(ptr);
	return (new_token);
}

static int	validate_input(const char *input)
{
	if (!input || *input == '\0')
		return (0);
	if (has_unclosed_quotes(input))
	{
		ft_putstr_fd("wizardshell: syntax error: unclosed quotes\n", 2);
		return (0);
	}
	return (1);
}

static int	process_tokens_l(const char **ptr, t_token **head)
{
	t_token	*new_token;

	while (**ptr)
	{
		skip_spaces(ptr);
		if (**ptr == '\0')
			break ;
		new_token = parse_next_token(ptr);
		if (!new_token)
		{
			free_tokens(*head);
			return (0);
		}
		add_token(head, new_token);
	}
	return (1);
}

/**
* main tokenization logic - converts input strings into linked list of tokens
* handles operators(|, <, >, <<, >>) and words (including quoted strings)
* pram input: input string to tokenize
* returns: Head of the token linked list, or NULL if input is empty
*/
t_token	*tokenize_input(const char *input)
{
	t_token		*head;
	const char	*ptr;

	if (!validate_input(input))
		return (NULL);
	head = NULL;
	ptr = input;
	if (!process_tokens_l(&ptr, &head))
		return (NULL);
	return (head);
}
