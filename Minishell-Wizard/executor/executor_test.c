/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_test.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:49:05 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/08 19:49:05 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * test_executor - Test function to demonstrate executor usage
 * @cmd_list: Parsed command pipeline
 * @envp: Environment variables
 * Return: Exit status
 */
int	test_executor(t_cmd *cmd_list, char **envp)
{
	t_exec_context	ctx;
	int				result;

	if (!cmd_list)
	{
		ft_putstr_fd("No commands to execute\n", 2);
		return (1);
	}

	/* Initialize execution context */
	if (init_execution_context(&ctx, envp) != 0)
	{
		ft_putstr_fd("Failed to initialize execution context\n", 2);
		return (1);
	}

	/* Set up signal handling */
	handle_signals_in_parent();

	/* Execute the pipeline */
	result = execute_pipeline(cmd_list, &ctx);

	/* Clean up */
	cleanup_execution_context(&ctx);

	return (result);
}

/**
 * print_test_header - Print test section header
 * @test_name: Name of the test
 */
static void	print_test_header(const char *test_name)
{
	printf("\n\033[34m=== %s ===\033[0m\n", test_name);
}

/**
 * print_test_result - Print test result
 * @expected: Expected exit status
 * @actual: Actual exit status
 */
static void	print_test_result(int expected, int actual)
{
	if (expected == actual)
		printf("\033[32m✅ Test passed (exit status: %d)\033[0m\n", actual);
	else
		printf("\033[31m❌ Test failed (expected: %d, got: %d)\033[0m\n", expected, actual);
}

/**
 * test_simple_command - Test simple command execution
 */
static void	test_simple_command(char **envp)
{
	t_token	*tokens;
	t_cmd	*cmd_list;
	int		result;

	print_test_header("Simple Command Test");
	
	/* Test: echo "Hello World" */
	printf("Testing: echo \"Hello World\"\n");
	tokens = tokenize_input("echo \"Hello World\"");
	if (!tokens)
	{
		printf("❌ Tokenization failed\n");
		return;
	}
	
	cmd_list = parse_command_line(tokens);
	if (!cmd_list)
	{
		printf("❌ Parsing failed\n");
		free_tokens(tokens);
		return;
	}
	
	result = test_executor(cmd_list, envp);
	print_test_result(0, result);
	
	free_cmd_list(cmd_list);
	free_tokens(tokens);
}

/**
 * test_builtin_commands - Test built-in commands
 */
static void	test_builtin_commands(char **envp)
{
	t_token	*tokens;
	t_cmd	*cmd_list;
	int		result;

	print_test_header("Built-in Commands Test");
	
	/* Test: pwd */
	printf("Testing: pwd\n");
	tokens = tokenize_input("pwd");
	if (tokens)
	{
		cmd_list = parse_command_line(tokens);
		if (cmd_list)
		{
			result = test_executor(cmd_list, envp);
			print_test_result(0, result);
			free_cmd_list(cmd_list);
		}
		free_tokens(tokens);
	}
	
	/* Test: echo with -n flag */
	printf("\nTesting: echo -n \"No newline\"\n");
	tokens = tokenize_input("echo -n \"No newline\"");
	if (tokens)
	{
		cmd_list = parse_command_line(tokens);
		if (cmd_list)
		{
			result = test_executor(cmd_list, envp);
			printf(" <- (should have no newline)\n");
			print_test_result(0, result);
			free_cmd_list(cmd_list);
		}
		free_tokens(tokens);
	}
}

/**
 * test_pipeline - Test pipeline execution
 */
static void	test_pipeline(char **envp)
{
	t_token	*tokens;
	t_cmd	*cmd_list;
	int		result;

	print_test_header("Pipeline Test");
	
	/* Test: echo "hello world" | grep "world" */
	printf("Testing: echo \"hello world\" | grep \"world\"\n");
	tokens = tokenize_input("echo \"hello world\" | grep \"world\"");
	if (!tokens)
	{
		printf("❌ Tokenization failed\n");
		return;
	}
	
	cmd_list = parse_command_line(tokens);
	if (!cmd_list)
	{
		printf("❌ Parsing failed\n");
		free_tokens(tokens);
		return;
	}
	
	result = test_executor(cmd_list, envp);
	print_test_result(0, result);
	
	free_cmd_list(cmd_list);
	free_tokens(tokens);
}

/**
 * test_redirections - Test input/output redirections
 */
static void	test_redirections(char **envp)
{
	t_token	*tokens;
	t_cmd	*cmd_list;
	int		result;

	print_test_header("Redirection Test");
	
	/* Create a test file first */
	system("echo 'test content' > /tmp/test_input.txt");
	
	/* Test: cat < /tmp/test_input.txt */
	printf("Testing: cat < /tmp/test_input.txt\n");
	tokens = tokenize_input("cat < /tmp/test_input.txt");
	if (tokens)
	{
		cmd_list = parse_command_line(tokens);
		if (cmd_list)
		{
			result = test_executor(cmd_list, envp);
			print_test_result(0, result);
			free_cmd_list(cmd_list);
		}
		free_tokens(tokens);
	}
	
	/* Test: echo "output test" > /tmp/test_output.txt */
	printf("\nTesting: echo \"output test\" > /tmp/test_output.txt\n");
	tokens = tokenize_input("echo \"output test\" > /tmp/test_output.txt");
	if (tokens)
	{
		cmd_list = parse_command_line(tokens);
		if (cmd_list)
		{
			result = test_executor(cmd_list, envp);
			print_test_result(0, result);
			printf("Check /tmp/test_output.txt for output\n");
			system("cat /tmp/test_output.txt");  /* Show the result */
			free_cmd_list(cmd_list);
		}
		free_tokens(tokens);
	}
	
	/* Clean up test files */
	system("rm -f /tmp/test_input.txt /tmp/test_output.txt");
}

/**
 * test_complex_pipeline - Test complex pipeline with redirections
 */
static void	test_complex_pipeline(char **envp)
{
	t_token	*tokens;
	t_cmd	*cmd_list;
	int		result;

	print_test_header("Complex Pipeline Test");
	
	/* Create test data */
	system("echo -e 'apple\\nbanana\\ncherry\\napricot' > /tmp/fruits.txt");
	
	/* Test: cat /tmp/fruits.txt | grep 'a' | sort > /tmp/sorted_fruits.txt */
	printf("Testing: cat /tmp/fruits.txt | grep 'a' | sort > /tmp/sorted_fruits.txt\n");
	tokens = tokenize_input("cat /tmp/fruits.txt | grep 'a' | sort > /tmp/sorted_fruits.txt");
	if (tokens)
	{
		cmd_list = parse_command_line(tokens);
		if (cmd_list)
		{
			result = test_executor(cmd_list, envp);
			print_test_result(0, result);
			printf("Result in /tmp/sorted_fruits.txt:\n");
			system("cat /tmp/sorted_fruits.txt");
			free_cmd_list(cmd_list);
		}
		free_tokens(tokens);
	}
	
	/* Clean up */
	system("rm -f /tmp/fruits.txt /tmp/sorted_fruits.txt");
}

/**
 * run_executor_tests - Run all executor tests
 */
void	run_executor_tests(char **envp)
{
	printf("\033[35m╔══════════════════════════════════════════════╗\n");
	printf("║              EXECUTOR TEST SUITE             ║\n");
	printf("╚══════════════════════════════════════════════╝\033[0m\n");
	
	test_simple_command(envp);
	test_builtin_commands(envp);
	test_pipeline(envp);
	test_redirections(envp);
	test_complex_pipeline(envp);
	
	printf("\n\033[36m=== All Executor Tests Completed ===\033[0m\n");
}

/**
 * main - Main function for testing the executor
 */
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	
	printf("Minishell Executor Test Program\n");
	printf("===============================\n");
	
	run_executor_tests(envp);
	
	return (0);
}
