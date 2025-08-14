/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:48:12 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/08 19:48:12 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * builtin_cd - Change directory command
 * argv: Command arguments
 * ctx: Execution context
 * Return: Exit status
 */
int	builtin_cd(char **argv, t_exec_context *ctx)
{
	char	*path;
	char	*oldpwd;
	char	cwd[PATH_MAX];
	(void)ctx;

	if (!argv[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		path = getenv("OLDPWD");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (1);
		}
		printf("%s\n", path);
	}
	else
		path = argv[1];
	if (getcwd(cwd, sizeof(cwd)))
		oldpwd = ft_strdup(cwd);
	else
		oldpwd = NULL;
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		free(oldpwd);
		return (1);
	}
	if (oldpwd)
	{
		setenv("OLDPWD", oldpwd, 1);
		free(oldpwd);
	}

	if (getcwd(cwd, sizeof(cwd)))
		setenv("PWD", cwd, 1);

	return (0);
}

/**
 * builtin_echo - Echo command with -n option support
 * argv: Command arguments
 * Return: Exit status
 */
int	builtin_echo(char **argv)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	if (argv[i] && ft_strcmp(argv[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}

	if (newline)
		printf("\n");

	return (0);
}

/**
 * builtin_env - Print environment variables
 * argv: Command arguments
 * ctx: Execution context
 * Return: Exit status
 */
int	builtin_env(char **argv, t_exec_context *ctx)
{
	int	i;

	(void)argv;

	if (!ctx->envp)
		return (1);
	i = 0;
	while (ctx->envp[i])
	{
		printf("%s\n", ctx->envp[i]);
		i++;
	}
	return (0);
}

/**
 * builtin_exit - Exit command
 * argv: Command arguments
 * ctx: Execution context
 * Return: Exit status (though it exits before returning)
 */
int	builtin_exit(char **argv, t_exec_context *ctx)
{
	int	exit_code;

	printf("exit\n");
	if (argv[1])
	{
		exit_code = ft_atoi(argv[1]);
		if (!ft_isdigit(argv[1][0]) && argv[1][0] != '-' && argv[1][0] != '+')
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit_code = 2;
		}
		if (argv[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
	}
	else
		exit_code = ctx->last_exit_status;
	cleanup_execution_context(ctx);
	exit(exit_code);
}

/**
 * builtin_export - Export environment variables (basic implementation)
 * argv: Command arguments
 * ctx: Execution context
 * Return: Exit status
 */
int	builtin_export(char **argv, t_exec_context *ctx)
{
	int		i;
	char	*equals;
	char	*key;
	char	*value;
	(void)ctx;

	if (!argv[1])
		return (builtin_env(argv, ctx));
	i = 1;
	while (argv[i])
	{
		equals = ft_strchr(argv[i], '=');
		if (equals)
		{
			key = ft_substr(argv[i], 0, equals - argv[i]);
			value = ft_strdup(equals + 1);
			if (key && value)
			{
				if (setenv(key, value, 1) != 0)
					perror("minishell: export");
			}
			free(key);
			free(value);
		}
		else
		{
			if (setenv(argv[i], "", 1) != 0)
				perror("minishell: export");
		}
		i++;
	}

	return (0);
}

/**
 * builtin_pwd - Print working directory
 * argv: Command arguments
 * Return: Exit status
 */
int	builtin_pwd(char **argv)
{
	char	cwd[PATH_MAX];

	(void)argv;

	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("minishell: pwd");
		return (1);
	}
}

/**
 * builtin_unset - Unset environment variables
 * argv: Command arguments
 * ctx: Execution context
 * Return: Exit status
 */
int	builtin_unset(char **argv, t_exec_context *ctx)
{
	int	i;
	(void)ctx;

	if (!argv[1])
		return (0);

	i = 1;
	while (argv[i])
	{
		if (unsetenv(argv[i]) != 0)
			perror("minishell: unset");
		i++;
	}

	return (0);
}
