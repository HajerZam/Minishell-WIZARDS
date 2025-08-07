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

char	*expand_variables(const char *input, t_env *env, int last_exit_status)
{
	t_var_data	data;
	char		*result;

	if (!input)
		return (NULL);
	data.result = ft_strdup("");
	if (!data.result)
		return (NULL);
	data.env = env;
	data.last_exit_status = last_exit_status;
	result = process_expansion_loop(input, &data);
	if (!result)
		free(data.result);
	return (result);
}
