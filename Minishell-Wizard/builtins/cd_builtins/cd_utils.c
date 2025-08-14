#include "../../minishell.h"

void print_cd_error(const char *path, const char *error_message)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	if (!path)
	{
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(error_message, STDERR_FILENO);
}

char	*expand_tilde(const char *path, t_env *env)
{
	char	*home;
	char	*expanded;
	char	*result;

	if (!path || !path[0] != '~')
		return (ft_strdup(path));
	home = get_home_path(env);
	if (!home)
		return (NULL);
	if (path[1] == '\0')
	{
		free(home);
		return (ft_strdup(home));
	}
	expanded = ft_strjoin(home, path + 1);
	free(home);
	if (!expanded)
		return (NULL);
	result = ft_strdup(expanded);
	free(expanded);
	return (result);
}

int	validate_cd_args(char **argv)
{
	int	ac;

	if (!argv)
		return (0);
	ac = 0;
	while (argv[ac])
		ac++;
	if (ac > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}
