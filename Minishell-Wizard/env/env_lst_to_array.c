/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lst_to_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 18:35:32 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 18:39:43 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env_array(char **envp, int count)
{
	int	i;

	if (!envp)
		return ;
	if (count == -1)
	{
		i = 0;
		while (envp[i])
		{
			free(envp[i]);
			i++;
		}
	}
	else
	{
		i = 0;
		while (i < count && envp[i])
		{
			free(envp[i]);
			i++;
		}
	}
	free(envp);
}

static int	count_exported_env_vars(t_env *env)
{
	t_env	*current;
	int		count;

	current = env;
	count = 0;
	while (current)
	{
		if (current->is_exported && current->value)
			count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_string(t_env *node)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(node->key, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, node->value);
	free(temp);
	return (result);
}

static int	fill_env_array(char **envp, t_env *env, int count)
{
	t_env	*current;
	int		i;

	current = env;
	i = 0;
	while (current && i < count)
	{
		if (current->is_exported && current->value)
		{
			envp[i] = create_env_string(current);
			if (!envp[i])
			{
				free_env_array(envp, i);
				return (0);
			}
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
	return (1);
}

char	**env_list_to_array(t_env *env)
{
	char	**envp;
	int		count;

	count = count_exported_env_vars(env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	if (!fill_env_array(envp, env, count))
		return (NULL);
	return (envp);
}
