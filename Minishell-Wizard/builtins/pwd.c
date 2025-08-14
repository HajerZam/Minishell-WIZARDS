/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:07:06 by halzamma          #+#    #+#             */
/*   Updated: 2025/07/29 12:07:06 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int    handle_pwd_output(char *directory)
{
    if (!directory)
    {
        perror("pwd");
        return (1);
    }
    ft_putendl_fd(directory, STDOUT_FILENO);
    free(directory);
    return (0);
}

int    builtin_pwd(void)
{
    char    *directory;

    directory = getcwd(NULL, 0);
    if (!directory)
    {
        perror("pwd");
        return (1);
    }
    return (handle_pwd_output(directory));
}
