/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:18:21 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/28 14:01:52 by halzamma         ###   ########.fr       */
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
char	*add_text_before_var(const char *input, t_var_data *data)
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
		return (new_result);
	}
	return (ft_strdup(data->result));
}

char	*expand_variables(const char *input, t_env *env, int last_exit_status)
{
	char		*result;
	char		*temp;
	size_t		i;
	size_t		start;
	t_var_data	data;

	if (!input)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	data.result = result;
	data.env = env;
	data.last_exit_status = last_exit_status;
	data.i = &i;
	data.start = &start;
	i = 0;
	start = 0;
	while (input[i])
	{
		if (input[i] == '$' && should_expand_variable(input, i))
		{
			temp = add_text_before_var(input, &data);
			if (!temp)
			{
				free(result);
				return (NULL);
			}
			if (temp != data.result)
			{
				free(result);
				result = temp;
				data.result = result;
			}
			temp = process_variable(input, &data);
			if (!temp)
			{
				free(result);
				return (NULL);
			}
			free(result);
			result = temp;
			data.result = result;
		}
		else
		{
			i++;
		}
	}
	temp = add_remaining_text(input, i, start, result);
	if (!temp)
		return (NULL);
	return (temp);
}
