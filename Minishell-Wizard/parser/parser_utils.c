#include "minishell.h"

int	handle_word_token(t_cmd *cmd, t_parser *parser, int *arg_index,
		int arg_count)
{
	if (*arg_index >= arg_count)
	{
		fprintf(stderr, "Too many arguments for command\n");
		parser->error = 1;
		return (0);
	}
	cmd->argv[*arg_index] = ft_strdup(parser->current->value);
		// try the dupstring thingy if needed
	if (!cmd->argv[*arg_index])
	{
		parser->error = 1;
		return (0);
	}
	(*arg_index)++;
	consume_token(parser);
	return (1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		i++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int	is_redirection(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT || type == REDIR_APPEND
		|| type == HEREDOC);
}

int	count_words_until_pipe(t_parser *parser)
{
	int		count;
	t_token	*current;

	count = 0;
	current = parser->current;
	if (!current)
		return (0);
	count = 0;
	while (current && current->type != PIPE)
	{
		if (current->type == WORD)
		{
			count++;
			current = current->next;
		}
		else if (is_redirection(current->type))
		{
			current = current->next;
			if (current && current->type == WORD)
				current = current->next;
		}
		else
			break ;
	}
	return (count);
}
