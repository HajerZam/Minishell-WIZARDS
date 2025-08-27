/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:59:35 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/27 15:14:14 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_argv(cmd->argv);
	if (cmd->heredoc_fd != -1)
		close(cmd->heredoc_fd);
	if (cmd->envp)
		free(cmd->envp);
	free(cmd);
}

void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmd_list;
	while (current)
	{
		next = current->next;
		free_cmd(current);
		current = next;
	}
}
