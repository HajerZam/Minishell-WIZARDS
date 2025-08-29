/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_before_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 11:56:45 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/29 11:56:45 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
