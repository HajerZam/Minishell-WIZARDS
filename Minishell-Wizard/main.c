/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:29:52 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/24 14:29:52 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*PS. the main is just set up for testing for now*/

int	main(void)
{
	char	*line;
	t_token	*tokens;

	init_signals();
	line = readline("minishell$ ");
	while (line != NULL)
	{
		if (*line)
			add_history(line);
		tokens = tokenize_input(line);
		print_tokens(tokens);
		free_tokens(tokens);
		free(line);
		line = readline("minishell$ ");
	}
	printf("exit\n");
	return (0);
}
