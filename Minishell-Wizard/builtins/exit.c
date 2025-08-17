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
/* DO NOT CALL exit() HERE - let main handle it */
/* This is handled in main.c process_command_line */
/*int builtin_exit(char **argv)
{
    int exit_code = 0;

    if (argv[1])
    {
        char *endptr;
        long val = strtol(argv[1], &endptr, 10);
        
        if (*endptr != '\0' || val < 0 || val > 255)
        {
            fprintf(stderr, "bash: exit: %s: numeric argument required\n", argv[1]);
            exit_code = 2;
        }
        else
        {
            exit_code = (int)val;
        }
    }
    printf("exit\n");
    return exit_code;
}
*/