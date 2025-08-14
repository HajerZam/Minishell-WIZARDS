#include "../../minishell.h"

char	*handle_cd_path(const char *path, t_env *env)
{
	char	*resolved_path;

	if (!path || !env)
		return (NULL);
	resolved_path = expand_tilde(path, env);
	if (!resolved_path)
	{
		print_cd_error(path, "No such file or directory");
		return (NULL);
	}
	return (resolved_path);
}

char	*handle_cd_oldpwd(t_env *env)
{
	char	*old_pwd;

	if (!env)
		return (NULL);
	old_pwd = getenv_from_list(env, "OLDPWD");
	if (!old_pwd)
	{
		print_cd_error(NULL, "OLDPWD not set");
		return (NULL);
	}
	return (ft_strdup(old_pwd));
}

char	*handle_cd_home(t_env *env)
{
	char	*home_path;

	if (!env)
		return (NULL);
	home_path = ft_stdup(getenv_from_list(env, "HOME"));
	if (!home_path)
	{
		print_cd_error(NULL, "HOME not set");
		return (NULL);
	}
	return (home_path);
}

char	*resolve_cd_target(char **argv, t_env *env)
{
	if (!argv[1])
		return (handle_cd_home(env));
	if (ft_strcmp(argv[1], "~") == 0)
		return (handle_cd_home(env));
	if (ft_strcmp(argv[1], "-") == 0)
		return (handle_cd_oldpwd(env));
	return (handle_cd_path(argv[1], env));
}

int	update_working_directory(const char *target_pwd)
{
	if (!target_pwd)
		return (1);
	if (chdir(target_pwd) == -1)
	{
		print_cd_error(target_pwd, "No such file or directory");
		return (1);
	}
	return (0);
}
