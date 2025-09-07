/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:29:31 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 09:16:41 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_signal_received;

/* Replace your handle_sigint_interactive function with this final version */

void	handle_sigint_interactive(int sig)
{
	(void)sig;
	
	/* Set the signal flag */
	g_signal_received = SIGINT;
	
	/* Write newline to move to next line */
	write(STDOUT_FILENO, "\n", 1);
	
	/* Only manipulate readline if we're actually in readline */
	if (RL_ISSTATE(RL_STATE_READCMD))
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/* Signal handler for execution mode (when running commands) */
void	handle_sigint_execution(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
}

/* Signal handler that does nothing (for ignoring signals) */
void	handle_sigquit_ignore(int sig)
{
	(void)sig;
	/* Do nothing - effectively ignores the signal */
}

/* Initialize signals for interactive mode (main shell loop) */
void	init_signals_interactive(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	/* Setup SIGINT handler for interactive mode */
	sa_int.sa_handler = handle_sigint_interactive;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);

	/* Ignore SIGQUIT in interactive mode */
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/* Initialize signals for command execution mode */
void	init_signals_execution(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	/* Setup SIGINT handler for execution mode */
	sa_int.sa_handler = handle_sigint_execution;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	/* Ignore SIGQUIT during execution */
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/* Initialize signals for child processes */
void	init_signals_child(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	/* Reset SIGINT to default behavior for child processes */
	sa_int.sa_handler = SIG_DFL;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	/* Reset SIGQUIT to default behavior for child processes */
	sa_quit.sa_handler = SIG_DFL;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/* Legacy function for backward compatibility */
void	init_signals(void)
{
	init_signals_interactive();
}
