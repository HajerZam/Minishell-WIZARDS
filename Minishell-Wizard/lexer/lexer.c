/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:27:41 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/24 14:27:41 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

/*  Function to create a new token, 
	iterate input, handle quotes, pipes, redirs, etc.*/

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

t_token	*tokenize_input(const char *input)
{
	t_token		*head;
	const char	*ptr;
	t_token		*new_token;

	head = NULL;
	ptr = input;
	if (!input || *input == '\0')
		return (NULL);
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
