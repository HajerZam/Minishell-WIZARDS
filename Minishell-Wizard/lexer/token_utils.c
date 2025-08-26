/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:32:41 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/26 14:44:11 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	skip_spaces(const char **ptr)
{
	while (**ptr && isspace(**ptr))
		(*ptr)++;
}

static void	handle_less_operator(const char **ptr, t_token *token)
{
	if (*(*ptr + 1) == '<')
	{
		token->type = HEREDOC;
		token->value = ft_strdup("<<");
		*ptr += 2;
	}
	else
	{
		token->type = REDIR_IN;
		token->value = ft_strdup("<");
		(*ptr)++;
	}
}

static void	handle_greater_operator(const char **ptr, t_token *token)
{
	if (*(*ptr + 1) == '>')
	{
		token->type = REDIR_APPEND;
		token->value = ft_strdup(">>");
		(*ptr) += 2;
	}
	else
	{
		token->type = REDIR_OUT;
		token->value = ft_strdup(">");
		(*ptr)++;
	}
}

/**
 * parses operator tokens from input (|, <, >, <<, >>)
 * handles both single and double-character operators
 * param ptr: Pointer to current position in input (updated after parsing)
 * returns: New token containing the parsed operator
 */
t_token	*parse_operator(const char **ptr)
{
	t_token	*token;

	token = create_token();
	if (**ptr == '|')
	{
		token->type = PIPE;
		token->value = ft_strdup("|");
		(*ptr)++;
	}
	else if (**ptr == '<')
		handle_less_operator(ptr, token);
	else if (**ptr == '>')
		handle_greater_operator(ptr, token);
	return (token);
}

void	free_tokens(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}
