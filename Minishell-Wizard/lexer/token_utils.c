/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:32:41 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/24 14:32:41 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token *create_token(void)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	token->value = NULL;
	token->next = NULL;
	return token;
}

void skip_spaces(const char **ptr)
{
	while (**ptr && isspace(**ptr))
		(*ptr)++;
}

int is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token *parse_operator(const char **ptr)
{
	t_token *token;

    token = create_token();
	if (**ptr == '|')
	{
		token->type = PIPE;
		token->value = strdup("|");
		(*ptr)++;
	}
	else if (**ptr == '<')
	{
		token->type = REDIR_IN;
		token->value = strdup("<");
		(*ptr)++;
	}
	else if (**ptr == '>')
	{
		if (*(*ptr + 1) == '>')
		{
			token->type = REDIR_APPEND;
			token->value = strdup(">>");
			(*ptr) += 2;
		}
		else
		{
			token->type = REDIR_OUT;
			token->value = strdup(">");
			(*ptr)++;
		}
	}
	return (token);
}

char *strip_quotes(const char *input)
{
    char *out = malloc(strlen(input) + 1);
    if (!out)
        return NULL;

    size_t i = 0, j = 0;
    char quote = 0;

    while (input[i])
    {
        if (!quote && (input[i] == '\'' || input[i] == '"'))
            quote = input[i]; // enter quote
        else if (quote && input[i] == quote)
            quote = 0; // exit quote
        else
            out[j++] = input[i];
        i++;
    }
    out[j] = '\0';
    return out;
}

t_token *parse_word(const char **ptr)
{
	t_token *token;
	t_lexer_state state;
	const char *start;

    token = create_token();
    state = STATE_GENERAL;
    start = *ptr;
	while (**ptr)
	{
		if (state == STATE_GENERAL)
		{
			if (**ptr == '\'')
				state = STATE_IN_SINGLE_QUOTE;
			else if (**ptr == '"')
				state = STATE_IN_DOUBLE_QUOTE;
			else if (isspace(**ptr) || is_operator(**ptr))
				break;
		}
		else if (state == STATE_IN_SINGLE_QUOTE && **ptr == '\'')
			state = STATE_GENERAL;
		else if (state == STATE_IN_DOUBLE_QUOTE && **ptr == '"')
			state = STATE_GENERAL;
		(*ptr)++;
	}
	token->type = WORD;
	char *raw = strndup(start, *ptr - start);
	token->value = strip_quotes(raw);
	free(raw);
	return (token);
}
