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

void	skip_spaces(const char **ptr)
{
	while (**ptr && isspace(**ptr))
		(*ptr)++;
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token	*parse_operator(const char **ptr)
{
	t_token	*token;

	token = create_token();
	if (**ptr == '|')
	{
		token->type = PIPE;
		token->value = strdup("|");
		(*ptr)++;
	}
	else if (**ptr == '<')
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
	else if (**ptr == '>')
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
	return (token);
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
