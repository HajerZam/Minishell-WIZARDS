/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:18:21 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/29 11:57:07 by halzamma         ###   ########.fr       */
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

static char	*init_expansion(void)
{
	char	*result;

	result = ft_strdup("");
	return (result);
}

static int	handle_variable_expansion(const char *input, t_var_data *data)
{
	char	*temp;

	temp = add_text_before_var(input, data);
	if (!temp)
		return (0);
	if (temp != data->result)
	{
		free(data->result);
		data->result = temp;
	}
	temp = process_variable(input, data);
	if (!temp)
		return (0);
	free(data->result);
	data->result = temp;
	return (1);
}

static void	setup_expansion_data(t_var_data *data, char *result, t_env *env,
		int last_exit_status)
{
	data->result = result;
	data->env = env;
	data->last_exit_status = last_exit_status;
}

static char	*process_expansion_loop(const char *input, t_var_data *data)
{
	while (input[*(data->i)])
	{
		if (input[*(data->i)] == '$'
			&& should_expand_variable(input, *(data->i)))
		{
			if (!handle_variable_expansion(input, data))
			{
				free(data->result);
				return (NULL);
			}
		}
		else
			(*(data->i))++;
	}
	return (add_remaining_text(input, *(data->i),
			*(data->start), data->result));
}

char	*expand_variables(const char *input, t_env *env, int last_exit_status)
{
	char		*result;
	size_t		i;
	size_t		start;
	t_var_data	data;

	if (!input)
		return (NULL);
	result = init_expansion();
	if (!result)
		return (NULL);
	setup_expansion_data(&data, result, env, last_exit_status);
	data.i = &i;
	data.start = &start;
	i = 0;
	start = 0;
	return (process_expansion_loop(input, &data));
}
