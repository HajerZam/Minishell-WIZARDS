#include "minishell.h"

static int	is_builtin_name(const char *name)
{
	return (ft_strcmp(name, "cd") == 0 || ft_strcmp(name, "echo") == 0
		|| ft_strcmp(name, "env") == 0 || ft_strcmp(name, "exit") == 0
		|| ft_strcmp(name, "export") == 0 || ft_strcmp(name, "pwd") == 0
		|| ft_strcmp(name, "unset") == 0);
}

int	is_builtin_command(char **argv)
{
	if (!argv || !argv[0])
		return (0);
	return (is_builtin_name(argv[0]));
}