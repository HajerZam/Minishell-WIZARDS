/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:24:52 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/28 13:59:16 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*process_valid_variable(const char *input, t_var_data *data,
									size_t var_len)
{
	char	*var_value;
	char	*new_result;

	var_value = get_variable_value(input + *(data->i) + 1,
			var_len, data->env, data->last_exit_status);
	if (!var_value)
		return (NULL);
	new_result = ft_strjoin(data->result, var_value);
	free(var_value);
	if (!new_result)
		return (NULL);
	free(data->result);
	*(data->i) += var_len + 1;
	return (new_result);
}

char	*process_variable(const char *input, t_var_data *data)
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
		if (!result)
			return (NULL);
		free(data->result);
		(*(data->i))++;
		return (result);
	}
}

char	*add_remaining_text(const char *input, size_t i,
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
		free(temp);
		if (!final_result)
		{
			free(result);
			return (NULL);
		}
		free(result);
		return (final_result);
	}
	return (result);
}
