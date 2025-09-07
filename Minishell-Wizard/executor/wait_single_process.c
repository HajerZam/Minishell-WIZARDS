#include "../minishell.h"

static int	get_exit_status(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			return (130);
		}
		else if (sig == SIGQUIT)
		{
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
			return (131);
		}
		else
			return (128 + sig);
	}
	else
		return (1);
}

static int	check_result(t_exec_context *ctx, int i, int cmd_count,
		pid_t result)
{
	int	status;
	int	last_status;

	status = 0;
	if (result == ctx->pids[i])
	{
		last_status = get_exit_status(status);
		if (i == cmd_count - 1)
			ctx->last_exit_status = last_status;
		return (0);
	}
	return (1);
}

static int	wait_single_process_loop(t_exec_context *ctx, int i, int cmd_count,
		int status)
{
	pid_t	result;

	while (1)
	{
		result = waitpid(ctx->pids[i], &status, 0);
		if (!check_result(ctx, i, cmd_count, result))
			break ;
		else if (result == -1)
		{
			if (errno == EINTR)
			{
				if (g_signal_received == SIGINT)
				{
					ctx->last_exit_status = 130;
					g_signal_received = 0;
				}
				continue ;
			}
			if (errno != ECHILD)
				perror("waitpid");
			break ;
		}
	}
	return (0);
}

int	wait_single_process(t_exec_context *ctx, int i, int cmd_count)
{
	int	status;

	status = 0;
	if (ctx->pids[i] <= 0)
		return (0);
	wait_single_process_loop(ctx, i, cmd_count, status);
	return (0);
}
