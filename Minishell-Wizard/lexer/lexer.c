/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:27:41 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/26 14:43:44 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

/**
* add a new token to the end of the token linked list
* parem head: pointer to the head of the token list
* parem new_token: token to the head of the token list
 */

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*temp;

	if (!*head)
		*head = new_token;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
	}
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
	t_token		*new_token;

	head = NULL;
	ptr = input;
	if (!input || *input == '\0')
		return (NULL);
	if (has_unclosed_quotes(input))
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		return (NULL);
	}
	while (*ptr)
	{
		skip_spaces(&ptr);
		if (*ptr == '\0')
			break ;
		if (is_operator(*ptr))
			new_token = parse_operator(&ptr);
		else
			new_token = parse_word(&ptr);
		add_token(&head, new_token);
	}
	return (head);
}
