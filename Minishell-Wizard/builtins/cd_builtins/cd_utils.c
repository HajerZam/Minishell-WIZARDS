/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 21:35:51 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/15 21:35:54 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void print_cd_error(const char *path, const char *error_message)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	if (path)
	{
		ft_putstr_fd((char *)path, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd((char *)error_message, STDERR_FILENO);
}

char	*expand_tilde(const char *path, t_env *env)
{
	char	*home;
	char	*expanded;

	if (!path || path[0] != '~')
		return (ft_strdup(path));
	home = getenv_from_list(env, "HOME");
	if (!home)
		return (NULL);
	if (path[1] == '\0')
		return (ft_strdup(home));
	expanded = ft_strjoin(home, path + 1);
	if (!expanded)
		return (NULL);
	return (expanded);
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
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}
