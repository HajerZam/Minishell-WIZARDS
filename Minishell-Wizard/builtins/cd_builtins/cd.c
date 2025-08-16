/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:06:30 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/15 18:37:17 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	update_pwd_env(t_env *env, const char *new_pwd)
{
	if (!env || !new_pwd)
		return (0);
	if (!update_env_value(&env, "PWD", new_pwd))
	{
		if (!add_env_var(&env, "PWD", new_pwd))
			return (0);
	}
	return (1);
}

int	update_olpwd_env(t_env *env, const char *old_pwd)
{
	if (!env || !old_pwd)
		return (0);
	if (!update_env_value(&env, "OLDPWD", old_pwd))
	{
		if (!add_env_var(&env, "OLDPWD", old_pwd))
			return (0);
	}
	return (1);
}

int	update_env_variables(t_env *env, const char *old_pwd)
{
	char	*new_pwd;
	int		result;

	if (!env)
		return (0);
	new_pwd = get_current_pwd();
	if (!new_pwd)
		return (0);
	result = 1;
	if (old_pwd && update_olpwd_env(env, old_pwd) == 0)
		result = 0;
	if (update_pwd_env(env, new_pwd) == 0)
		result = 0;
	free(new_pwd);
	return (result);
}

char	*get_current_pwd(void)
{
	char	*pwd;
	char	*result;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (NULL);
	result = ft_strdup(pwd);
	return (result);
}

int	builtin_cd(char **argv, t_env *env)
{
	char	*old_pwd;
	char	*target_pwd;
	int		result;

	if (!validate_cd_args(argv) || !env)
		return (1);
	old_pwd = get_current_pwd();
	target_pwd = resolve_cd_target(argv, env);
	if (!target_pwd)
	{
		free(old_pwd);
		return (1);
	}
	result = update_working_directory(target_pwd);
	if (result == 0)
		update_env_variables(env, old_pwd);
	free(old_pwd);
	free(target_pwd);
	return (result);
}
