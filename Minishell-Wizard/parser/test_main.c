/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:00:12 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/08 14:43:14 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* test_main.c - Test program to visualize execution tree (Norma 42) */

#include "../minishell.h"

/* Test cases for parser - FIXED: Properly formatted string array */
const char		*g_test_cases[] = {
	"ls", 
	"ls -la", 
	"echo hello world",
	"cat < input.txt", 
	"sort < data.txt", 
	"ls > output.txt",
	"echo hello >> log.txt", 
	"cat << EOF", 
	"sort << END",
	"ls | grep txt", 
	"cat file | sort", 
	"ls -la | grep txt | sort",
	"cat input | sort | uniq | wc -l",
	"cat < input.txt | sort > output.txt", 
	"ls -la | grep txt > filtered.txt", 
	NULL
};

const char		*g_error_test_cases[] = {
	"|", 
	"ls |", 
	"ls | | grep", 
	"ls >",
	"< input.txt", 
	"ls > > output.txt", 
	NULL
};

/**
 * test_single_case - Test parsing of a single command line
 * @input: Command line to test
 * @should_succeed: 1 if parsing should succeed, 0 if it should fail
 */
void	test_single_case(const char *input, int should_succeed)
{
   t_token *tokens;
    t_cmd *cmd_list;

    printf("\n\033[34mTesting: \033[0m'%s'\n", input);
    printf("Expected: %s\n", should_succeed ? "SUCCESS" : "FAILURE");
    
    // Add debug output
    printf("Debug: Input length = %zu\n", ft_strlen(input));
    
    tokens = tokenize_input(input);
    if (!tokens)
    {
        printf("\033[31m❌ Tokenization failed for input: '%s'\n\033[0m", input);
        if (should_succeed)
            printf("This should have succeeded!\n");
        return;
    }
    
    printf("Tokens: ");
    print_tokens(tokens);
    
    cmd_list = parse_command_line(tokens);
    if (cmd_list && should_succeed)
    {
        printf("\033[32m✅ Parsing succeeded\n\033[0m");
        print_pipeline_summary(cmd_list);
        print_execution_tree(cmd_list);
        if (validate_pipeline(cmd_list))
            printf("\033[32m✅ Pipeline validation passed\n\033[0m");
        else
            printf("\033[33m⚠️  Pipeline validation failed\n\033[0m");
    }
    else if (!cmd_list && !should_succeed)
        printf("\033[32m✅ Correctly rejected invalid syntax\n\033[0m");
    else if (cmd_list && !should_succeed)
    {
        printf("\033[31m❌ Should have failed but succeeded\n\033[0m");
        print_execution_tree(cmd_list);
    }
    else
        printf("\033[31m❌ Parsing failed unexpectedly\n\033[0m");    
    free_cmd_list(cmd_list);
    free_tokens(tokens);
    printf("\033[36m----------------------------------------\n\033[0m");
}

/**
 * run_valid_tests - Run tests that should succeed
 */
static void	run_valid_tests(void)
{
	int	i;

	printf("\033[36m\n=== VALID SYNTAX TESTS ===\n\033[0m");
	i = 0;
	while (g_test_cases[i])
	{
		test_single_case(g_test_cases[i], 1);
		i++;
	}
}

/**
 * run_error_tests - Run tests that should fail
 */
static void	run_error_tests(void)
{
	int	i;

	printf("\033[36m\n=== ERROR HANDLING TESTS ===\n\033[0m");
	i = 0;
	while (g_error_test_cases[i])
	{
		test_single_case(g_error_test_cases[i], 0);
		i++;
	}
}

/**
 * run_all_tests - Run all test cases
 */
void	run_all_tests(void)
{
	printf("\033[35m╔═══════════════════════════════════════╗\n");
	printf("║          PARSER TEST SUITE            ║\n");
	printf("╚═══════════════════════════════════════╝\n\033[0m");
	run_valid_tests();
	run_error_tests();
}

/**
 * interactive_loop - Main interactive loop
 */
static void interactive_loop(void)
{
    char *input;
    
    while (1)
    {
        input = readline("\033[32mminishell> \033[0m");
        if (!input)  // Ctrl+D
        {
            printf("\n");
            break;
        }
        
        // ENHANCED DEBUG: Print raw input details
        printf("DEBUG: Got input: '%s'\n", input);
        printf("DEBUG: Input length: %zu\n", ft_strlen(input));
        printf("DEBUG: Input address: %p\n", (void*)input);
        
        // Print each character with hex values
        printf("DEBUG: Character breakdown: ");
        for (size_t i = 0; i < ft_strlen(input); i++)
        {
            printf("[%zu]='%c'(0x%02X) ", i, input[i], (unsigned char)input[i]);
        }
        printf("\n");
        
        if (ft_strlen(input) == 0)  // Empty input
        {
            printf("DEBUG: Empty input, continuing\n");
            free(input);
            continue;
        }
        
        add_history(input);
        
        // ENHANCED DEBUG: Multiple comparison methods
        printf("DEBUG: About to compare strings...\n");
        
        // Method 1: ft_strcmp
        int cmp_result = ft_strcmp(input, "exit");
        printf("DEBUG: ft_strcmp('%s', 'exit') = %d\n", input, cmp_result);
        
        // Method 2: Manual character-by-character comparison
        printf("DEBUG: Manual comparison:\n");
        printf("  input[0]='%c'(0x%02X), 'e'=0x65: %s\n", 
               input[0], (unsigned char)input[0], 
               input[0] == 'e' ? "MATCH" : "NO_MATCH");
        printf("  input[1]='%c'(0x%02X), 'x'=0x78: %s\n", 
               input[1], (unsigned char)input[1], 
               input[1] == 'x' ? "MATCH" : "NO_MATCH");
        printf("  input[2]='%c'(0x%02X), 'i'=0x69: %s\n", 
               input[2], (unsigned char)input[2], 
               input[2] == 'i' ? "MATCH" : "NO_MATCH");
        printf("  input[3]='%c'(0x%02X), 't'=0x74: %s\n", 
               input[3], (unsigned char)input[3], 
               input[3] == 't' ? "MATCH" : "NO_MATCH");
        
        // Method 3: Standard library strcmp for comparison
        printf("DEBUG: stdlib strcmp('%s', 'exit') = %d\n", input, strcmp(input, "exit"));
        
        // Check the actual condition
        printf("DEBUG: Condition check: cmp_result == 0 is %s\n", 
               cmp_result == 0 ? "TRUE" : "FALSE");
        
        if (cmp_result == 0)
        {
            printf("DEBUG: Exit condition triggered!\n");
            printf("DEBUG: This should only happen for 'exit' command\n");
            free(input);
            break;
        }
        
        printf("DEBUG: Not exit command, proceeding to test_single_case\n");
        test_single_case(input, 1);
        free(input);
    }
}

/**
 * interactive_mode - Interactive testing mode
 */
void	interactive_mode(void)
{
	printf("\033[35m╔═══════════════════════════════════════╗\n");
	printf("║       INTERACTIVE PARSER TEST         ║\n");
	printf("║   Type commands to see execution tree ║\n");
	printf("║          (type 'exit' to quit)        ║\n");
	printf("╚═══════════════════════════════════════╝\n\033[0m");
	interactive_loop();
	printf("Goodbye!\n");
}

/**
 * print_usage - Print usage information
 */
void	print_usage(const char *program_name)
{
	printf("Usage: %s [options]\n", program_name);
	printf("Options:\n");
	printf("  -t, --test     Run all test cases\n");
	printf("  -i, --interactive  Interactive mode\n");
	printf("  -h, --help     Show this help\n");
	printf("\n");
	printf("If no options provided, runs interactive mode by default.\n");
}

/**
 * parse_arguments - Parse command line arguments
 * @argc: Argument count
 * @argv: Argument values
 *
 * Return: 0 for interactive, 1 for test, 2 for help, -1 for error
 */
static int	parse_arguments(int argc, char **argv)
{
	if (argc > 1)
	{
		if (ft_strcmp(argv[1], "-t") == 0 || ft_strcmp(argv[1], "--test") == 0)
			return (1);
		else if (ft_strcmp(argv[1], "-i") == 0 || ft_strcmp(argv[1],
				"--interactive") == 0)
			return (0);
		else if (ft_strcmp(argv[1], "-h") == 0 || ft_strcmp(argv[1],
				"--help") == 0)
			return (2);
		else
		{
			printf("Unknown option: %s\n", argv[1]);
			return (-1);
		}
	}
	return (0);
}

/**
 * main - Entry point for parser testing
 */
int	main(int argc, char **argv)
{
	int	mode;

	mode = parse_arguments(argc, argv);
	if (mode == 1)
	{
		run_all_tests();
		return (0);
	}
	else if (mode == 0)
	{
		interactive_mode();
		return (0);
	}
	else if (mode == 2)
	{
		print_usage(argv[0]);
		return (0);
	}
	else if (mode == -1)
	{
		print_usage(argv[0]);
		return (1);
	}
	interactive_mode();
	return (0);
}

static int	validate_single_command(t_cmd *current, int cmd_position)
{
	if (!current->argv || !current->argv[0])
	{
		ft_putstr_fd("Error: Empty command at position ", 2);
		ft_putnbr_fd(cmd_position, 2);
		ft_putstr_fd("\n", 2);
		return (0);
	}
	return (1);
}

int	validate_pipeline(t_cmd *cmd_list)
{
	t_cmd	*current;
	int		cmd_position;

	if (!cmd_list)
		return (0);
	current = cmd_list;
	cmd_position = 1;
	while (current)
	{
		if (!validate_single_command(current, cmd_position))
			return (0);
		current = current->next;
		cmd_position++;
	}
	return (1);
}

static void	print_cmd_basic(t_cmd *cmd)
{
	int	i;

	printf("│  argv: [");
	if (!cmd)
		printf("(NULL_CMD)]\n");
	else if (!cmd->argv)
		printf("(NULL_ARGV)]\n");
	else if (!cmd->argv[0])
		printf("(EMPTY_ARGV)]\n");
	else
	{
		i = 0;
		while (cmd->argv[i] && i < 50)
		{
			printf("'%s'", cmd->argv[i]);
			i++;
			if (cmd->argv[i])
				printf(", ");
		}
		if (i >= 50)
			printf(", ...(too_many_Args)");
		printf("]\n");
	}
}

/**
 * print_cmd_fds - Print file descriptor info
 * @cmd: Command to print
 */
static void	print_cmd_fds(t_cmd *cmd)
{
	printf("│  input_fd: %d", cmd->input_fd);
	if (cmd->input_fd > 2)
		printf(" (file/pipe)");
	else if (cmd->input_fd == -2)
		printf(" (file redirect)");
	else if (cmd->input_fd == -4)
		printf(" (heredoc)");
	printf("\n");
	printf("│  output_fd: %d", cmd->output_fd);
	if (cmd->output_fd > 2)
		printf(" (file/pipe)");
	else if (cmd->output_fd == -2)
		printf(" (file redirect)");
	else if (cmd->output_fd == -3)
		printf(" (append redirect)");
	printf("\n");
}

/**
 * print_cmd_flags - Print command flags
 * @cmd: Command to print
 */
static void	print_cmd_flags(t_cmd *cmd)
{
	if (cmd->is_heredoc)
		printf("│  📄 Uses heredoc\n");
	if (cmd->is_builtin)
		printf("│  ⚙️  Is builtin\n");
}

/**
 * print_cmd_tree - Print command structure as a tree
 * @cmd: Command to print
 * @depth: Indentation depth
 */
void	print_cmd_tree(t_cmd *cmd, int depth)
{
	int	i;

	if (!cmd)
	{
		i = 0;
		while (i < depth * 2)
		{
			printf(" ");
			i++;
		}
		printf("(null command)\n");
		return ;
	}
	i = 0;
	while (i < depth * 2)
	{
		printf(" ");
		i++;
	}
	printf("┌─ Command:\n");
	print_cmd_basic(cmd);
	print_cmd_fds(cmd);
	print_cmd_flags(cmd);
	if (cmd->next)
	{
		i = 0;
		while (i < depth * 2)
		{
			printf(" ");
			i++;
		}
		printf("│\n");
		i = 0;
		while (i < depth * 2)
		{
			printf(" ");
			i++;
		}
		printf("├─ PIPE TO:\n");
		print_cmd_tree(cmd->next, depth + 1);
	}
	else
	{
		i = 0;
		while (i < depth * 2)
		{
			printf(" ");
			i++;
		}
		printf("└─ (end of pipeline)\n");
	}
}

/**
 * print_execution_tree - Print entire execution tree
 * @cmd_list: First command in pipeline
 */
void	print_execution_tree(t_cmd *cmd_list)
{
	printf("\n\033[36m═══ EXECUTION TREE ═══\033[0m\n");
	if (!cmd_list)
	{
		printf("(empty tree)\n");
		return ;
	}
	print_cmd_tree(cmd_list, 0);
	printf("\033[36m═══════════════════════\033[0m\n\n");
}

/**
 * count_pipeline_commands - Count commands in pipeline
 * @cmd_list: First command in pipeline
 *
 * Return: Number of commands
 */
static int	count_pipeline_commands(t_cmd *cmd_list)
{
	t_cmd	*current;
	int		count;

	current = cmd_list;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/**
 * print_pipeline_summary - Print a one-line summary of the pipeline
 * @cmd_list: First command in pipeline
 */
void	print_pipeline_summary(t_cmd *cmd_list)
{
	t_cmd	*current;
	int		cmd_count;

	printf("\033[33mPipeline: \033[0m");
	current = cmd_list;
	while (current)
	{
		if (current->argv && current->argv[0])
			printf("%s", current->argv[0]);
		else
			printf("(empty)");
		if (current->next)
			printf(" | ");
		current = current->next;
	}
	cmd_count = count_pipeline_commands(cmd_list);
	printf(" (%d command", cmd_count);
	if (cmd_count != 1)
		printf("s");
	printf(")\n");
}

/**
 * cleanup_parser_error - Clean up parser state on error
 * @parser: Parser with error
 * @partial_cmd: Partially built command (if any)
 */
void	cleanup_parser_error(t_parser *parser, t_cmd *partial_cmd)
{
	if (partial_cmd)
		free_cmd_list(partial_cmd);
	parser->cmd_list = NULL;
	parser->error = 1;
}

/**
 * find_last_cmd - Find last command in pipeline
 * @head: First command in pipeline
 *
 * Return: Last command in pipeline
 */
static t_cmd	*find_last_cmd(t_cmd *head)
{
	t_cmd	*current;

	if (!head)
		return (NULL);
	current = head;
	while (current->next)
		current = current->next;
	return (current);
}

/**
 * safe_cmd_append - Safely append command to pipeline
 * @head: Pointer to first command in pipeline
 * @new_cmd: Command to append
 */
void	safe_cmd_append(t_cmd **head, t_cmd *new_cmd)
{
	t_cmd *last;

	if (!head || !new_cmd)
		return ;
	if (!*head)
	{
		*head = new_cmd;
		return ;
	}
	last = find_last_cmd(*head);
	last->next = new_cmd;
}