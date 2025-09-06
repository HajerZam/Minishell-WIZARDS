/* Add to env_utils.c - Convert t_env list to char** array */

#include "../minishell.h"

void	free_env_array(char **envp, int count)
{
	int	i;

	if (!envp)
		return ;
	
	/* If count is -1, count until NULL */
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

char	**env_list_to_array(t_env *env)
{
	char	**envp;
	t_env	*current;
	int		count;
	int		i;

	count = count_exported_env_vars(env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	
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
				return (NULL);
			}
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}