/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:24:52 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/07 18:40:16 by halzamma         ###   ########.fr       */
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

static char	*handle_valid_variable(const char *input, t_var_data *data,
								char *result_with_text, size_t var_len)
{
	char	*var_value;
	char	*final_result;

	var_value = get_variable_value(input + *(data->i) + 1,
			var_len, data->env, data->last_exit_status);
	if (!var_value)
	{
		free(result_with_text);
		return (NULL);
	}
	final_result = ft_strjoin(result_with_text, var_value);
	free(var_value);
	free(result_with_text);
	if (!final_result)
		return (NULL);
	*(data->i) += var_len + 1;
	return (final_result);
}

static char	*handle_invalid_variable(char *result_with_text, t_var_data *data)
{
	char	*final_result;

	final_result = ft_strjoin(result_with_text, "$");
	free(result_with_text);
	if (!final_result)
		return (NULL);
	(*(data->i))++;
	return (final_result);
}

char	*process_variable(const char *input, t_var_data *data)
{
	char	*result_with_text;
	size_t	var_len;

	result_with_text = add_text_before_var(input, data);
	if (!result_with_text)
		return (NULL);
	var_len = get_var_name_len(input + *(data->i) + 1);
	if (var_len > 0)
		return (handle_valid_variable(input, data, result_with_text, var_len));
	else
		return (handle_invalid_variable(result_with_text, data));
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
