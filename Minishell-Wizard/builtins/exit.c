/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:06:55 by halzamma          #+#    #+#             */
/*   Updated: 2025/07/29 12:06:55 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_exit(char **argv)
{
    int	exit_code;

    if (!argv || !argv[0])
    {
        printf("exit\n");
        exit(0);
    }
    if (ft_strcmp(argv[0], "exit") != 0)
    {
        fprintf(stderr, "bash: exit: %s: numeric argument required\n", argv[0]);
        return (1);
    }
    if (argv[1])
    {
        exit_code = ft_atoi(argv[1]);
        if (exit_code < 0 || exit_code > 255)
            exit_code = 255;
    }
    else
        exit_code = 0;
    printf("exit\n");
    exit(exit_code);
}
