/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:06:46 by halzamma          #+#    #+#             */
/*   Updated: 2025/07/29 12:06:46 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int    builtin_echo(char **argv)
{
    int	i;
    int	newline;

    if (!argv || !argv[0])
    {
        printf("\n");
        return (0);
    }
    newline = 1;
    i = 1;
    if (ft_strcmp(argv[1], "-n") == 0)
    {
        newline = 0;
        i++;
    }
    while (argv[i])
    {
        printf("%s", argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    return (0);
}
