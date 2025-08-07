/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_lexer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:34:38 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/24 14:34:38 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h> // for printf

const char	*token_type_to_str(t_token_type type)
{
	switch (type)
	{
		case WORD:          return "WORD";
		case PIPE:          return "PIPE";
		case REDIR_IN:      return "REDIR_IN";
		case REDIR_OUT:     return "REDIR_OUT";
		case REDIR_APPEND:  return "REDIR_APPEND";
		case HEREDOC:       return "HEREDOC";
		default:            return "UNKNOWN";
	}
}

void	print_tokens(t_token *head)
{
	while (head)
	{
		printf("[%-12s] \"%s\"\n", token_type_to_str(head->type), head->value);
		head = head->next;
	}
}

void	free_tokens(t_token *head)
{
	t_token *tmp;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->value);
		free(tmp);
	}
}
