/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:29:31 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/15 16:47:15 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_signal_received;

void	handle_sigint_interactive(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	if (RL_ISSTATE(RL_STATE_READCMD))
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_sigint_execution(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
}

void	handle_sigquit_ignore(int sig)
{
	(void)sig;
}

void	init_signals(void)
{
	init_signals_interactive();
}
