/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:59:35 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/28 21:15:42 by halzamma         ###   ########.fr       */
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
    int i;

    if (!cmd)
        return ;
    if (cmd->argv)
    {
        i = 0;
        while (cmd->argv[i])
        {
            free(cmd->argv[i]);
            i++;
        }
        free(cmd->argv);
    }
    if (cmd->input_fd > 2 && cmd->input_fd != -1)
        close(cmd->input_fd);
    if (cmd->output_fd > 2 && cmd->output_fd != -1)
        close(cmd->output_fd);
    if (cmd->heredoc_fd != -1)
        close(cmd->heredoc_fd);
    free(cmd);
}

void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd *current;
	t_cmd *next;

	current = cmd_list;
	while (current)
	{
		next = current->next;
		free_cmd(current);
		current = next;
	}
}
