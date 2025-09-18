/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strspn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 21:07:05 by halzamma          #+#    #+#             */
/*   Updated: 2025/09/18 21:07:25 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

size_t	ft_strspn(const char *str, const char *accept)
{
	const char	*ptr;
	const char	*accept_ptr;
	size_t		count;

	count = 0;
	ptr = str;
	while (*ptr)
	{
		accept_ptr = accept;
		while (*accept_ptr)
		{
			if (*ptr == *accept_ptr)
				break ;
			accept_ptr++;
		}
		if (!*accept_ptr)
			return (count);
		count++;
		ptr++;
	}
	return (count);
}
