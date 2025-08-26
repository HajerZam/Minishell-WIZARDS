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

static int is_valid_n_flag(const char *arg)
{
    int i;
    
    if (!arg || arg[0] != '-' || arg[1] == '\0')
        return (0);
    
    i = 1;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

int builtin_echo(char **argv)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
    if (!argv || !argv[0])
    {
        printf("\n");
        return (0);
    }
    while (argv[i] && is_valid_n_flag(argv[i]))
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
