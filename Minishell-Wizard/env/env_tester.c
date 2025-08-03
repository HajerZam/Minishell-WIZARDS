/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tester.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:17:35 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/01 16:17:35 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
#include "../minishell.h"
#include <stdio.h>

void test_env_expansion(char **envp) // FIXED: Added envp parameter
{
    t_env *env = init_env(envp);
    char *result;
    
    if (!env)
    {
        printf("Error: Failed to initialize environment\n");
        return;
    }
    
    g_last_exit_status = 42;

    printf("=== Testing Variable Expansion ===\n");
    
    result = expand_variables("Hello $USER", env);
    printf("\"Hello $USER\" -> \"%s\"\n", result ? result : "NULL");
    free(result);
    
    result = expand_variables("Exit: $?", env);
    printf("\"Exit: $?\" -> \"%s\"\n", result ? result : "NULL");
    free(result);
    
    result = expand_variables("'$HOME'", env);
    printf("\"'$HOME'\" -> \"%s\"\n", result ? result : "NULL");
    free(result);
    
    result = expand_variables("\"$HOME\"", env);
    printf("\"\\\"$HOME\\\"\" -> \"%s\"\n", result ? result : "NULL");
    free(result);
    
    result = expand_variables("$HOME/documents", env);
    printf("\"$HOME/documents\" -> \"%s\"\n", result ? result : "NULL");
    free(result);
    
    result = expand_variables("No vars here", env);
    printf("\"No vars here\" -> \"%s\"\n", result ? result : "NULL");
    free(result);
    
    printf("\n=== Testing Environment Functions ===\n");
    
    // Test getenv_from_list
    printf("USER = %s\n", getenv_from_list(env, "USER"));
    printf("HOME = %s\n", getenv_from_list(env, "HOME"));
    printf("NONEXISTENT = %s\n", getenv_from_list(env, "NONEXISTENT"));
    
    // Test update_env_value
    printf("\nTesting update_env_value:\n");
    printf("Before: TEST_VAR = %s\n", getenv_from_list(env, "TEST_VAR"));
    update_env_value(&env, "USER", "test_user");
    printf("After updating USER: %s\n", getenv_from_list(env, "USER"));
    
    // Test add_env_var
    add_env_var(&env, "TEST_VAR", "test_value");
    printf("After adding TEST_VAR: %s\n", getenv_from_list(env, "TEST_VAR"));
    
    // Test unset_env_var
    unset_env_var(&env, "TEST_VAR");
    printf("After unsetting TEST_VAR: %s\n", getenv_from_list(env, "TEST_VAR"));
    
    free_env(env);
}

// Main function for standalone testing
int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    
    test_env_expansion(envp);
    return (0);
}
*/