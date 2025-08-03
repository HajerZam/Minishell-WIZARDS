/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:31:07 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/01 16:31:07 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Helper function to check if we're inside single quotes */
int	is_in_single_quotes(const char *str, size_t pos)
{
	size_t	i;
	int		in_quotes;

	i = 0;
	in_quotes = 0;
	while (i < pos)
	{
		if (str[i] == '\'' && (i == 0 || str[i - 1] != '\\'))
			in_quotes = !in_quotes;
		i++;
	}
	return (in_quotes);
}

/* Helper function to get variable name length
Handle $? special case
Variable names: alphanumeric + underscore, must start with letter/underscore */
size_t	get_var_name_len(const char *str)
{
	size_t	len;

	len = 0;
	if (str[0] == '?')
		return (1);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

/* Helper function to get variable value 
 Handle $? special case
 Create null-terminated key
 Return copy of value or empty string if not found
*/
char	*get_variable_value(const char *var_name,
						size_t var_len, t_env *env)
{
	char	*key;
	char	*value;
	char	*exit_str;

	if (var_len == 1 && var_name[0] == '?')
	{
		exit_str = ft_itoa(g_last_exit_status);
		return (exit_str);
	}
	key = ft_substr(var_name, 0, var_len);
	if (!key)
		return (NULL);
	value = getenv_from_list(env, key);
	free(key);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

char	*expand_variable(const char *input, t_env *env)
{
	return (expand_variables(input, env));
}

int	should_expand_variable(const char *input, size_t i)
{
	return (input[i] == '$' && !is_in_single_quotes(input, i)
		&& (i == 0 || input[i - 1] != '\\'));
}
