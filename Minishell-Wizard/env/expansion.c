/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:18:21 by halzamma          #+#    #+#             */
/*   Updated: 2025/07/29 12:18:21 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Main expansion logic
This will expand the input string according to the rules of the shell
handling variables, special characters, etc.
It will return a new string with the expanded value
If the input is NULL, it will return NULL
If memory allocation fails, it will return NULL
*/
static char	*add_text_before_var(const char *input, t_var_data *data)
{
	char	*temp;
	char	*new_result;

	if (*(data->i) > *(data->start))
	{
		temp = ft_substr(input, *(data->start), *(data->i) - *(data->start));
		if (!temp)
			return (NULL);
		new_result = ft_strjoin(data->result, temp);
		free(temp);
		if (!new_result)
			return (NULL);
		free(data->result);
		return (new_result);
	}
	return (data->result);
}

static char	*process_valid_variable(const char *input, t_var_data *data,
									size_t var_len)
{
	char	*var_value;
	char	*new_result;

	var_value = get_variable_value(input + *(data->i) + 1, var_len, data->env);
	if (!var_value)
		return (NULL);
	new_result = ft_strjoin(data->result, var_value);
	free(var_value);
	*(data->i) += var_len + 1;
	return (new_result);
}

static char	*process_variable(const char *input, t_var_data *data)
{
	char	*result;
	size_t	var_len;

	result = add_text_before_var(input, data);
	if (!result)
		return (NULL);
	data->result = result;
	var_len = get_var_name_len(input + *(data->i) + 1);
	if (var_len > 0)
		return (process_valid_variable(input, data, var_len));
	else
	{
		result = ft_strjoin(data->result, "$");
		(*(data->i))++;
		return (result);
	}
}

static char	*add_remaining_text(const char *input, size_t i,
							size_t start, char *result)
{
	char	*temp;
	char	*final_result;

	if (i > start)
	{
		temp = ft_substr(input, start, i - start);
		if (!temp)
		{
			free(result);
			return (NULL);
		}
		final_result = ft_strjoin(result, temp);
		free(result);
		free(temp);
		return (final_result);
	}
	return (result);
}

static char	*process_expansion_loop(const char *input, t_var_data *data)
{
	char	*temp;
	size_t	i;
	size_t	start;

	i = 0;
	start = 0;
	data->i = &i;
	data->start = &start;
	while (input[i])
	{
		if (should_expand_variable(input, i))
		{
			temp = process_variable(input, data);
			if (!temp)
				return (NULL);
			data->result = temp;
			start = i;
		}
		else
			i++;
	}
	return (add_remaining_text(input, i, start, data->result));
}

char	*expand_variables(const char *input, t_env *env)
{
	t_var_data	data;
	char		*result;

	if (!input)
		return (NULL);
	data.result = ft_strdup("");
	if (!data.result)
		return (NULL);
	data.env = env;
	result = process_expansion_loop(input, &data);
	if (!result)
		free(data.result);
	return (result);
}
