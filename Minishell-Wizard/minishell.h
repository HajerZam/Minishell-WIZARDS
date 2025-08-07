#ifndef MINISHELL_H
# define MINISHELL_H

# include <include/libft.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef enum e_lexer_state
{
	STATE_GENERAL,
	STATE_IN_SINGLE_QUOTE,
	STATE_IN_DOUBLE_QUOTE
}	t_lexer_state;

typedef struct s_parse_vars
{
	const char		*input;
	t_token			*token;
	char			*buffer;
	size_t			i;
	size_t			j;
	t_lexer_state	state;
}	t_parse_vars;

typedef struct s_redirect {
    t_token_type type;
    char *filename;
    int fd;
    struct s_redirect *next;
} t_redirect;

typedef struct s_cmd {
    char **argv;
    int input_fd;
    int output_fd;
    struct s_cmd *next;
	struct s_token *tokens;
	int heredoc_fd;
	int is_heredoc;
	int is_builtin;
	char *envp;
	int exit_status;
} t_cmd;

typedef struct s_parser {
    t_token *tokens;
    t_token *current;
    t_cmd *cmd_list;
    int error;
} t_parser;

/* ============================================================================
 * PARSER FUNCTION DECLARATIONS
 * ============================================================================ */

/* parser_main.c */
t_cmd		*parse_command_line(t_token *tokens);
void		init_parser(t_parser *parser, t_token *tokens);
int			validate_syntax(t_token *tokens);

/* parser_utils.c */
t_token		*peek_token(t_parser *parser);
t_token		*consume_token(t_parser *parser);
int			expect_token(t_parser *parser, t_token_type type);
int			is_redirection(t_token_type token);
int		 	handle_word_token(t_cmd *cmd, t_parser *parser, int *arg_index, int arg_count);
int			count_words_until_pipe(t_parser *parser);
void		print_cmd_tree(t_cmd *cmd, int depth);
void		print_execution_tree(t_cmd *cmd_list);
void		print_pipeline_summary(t_cmd *cmd_list);
int			ft_strcmp(const char *s1, const char *s2);
/* parser_command.c */
t_cmd		*parse_command(t_parser *parser);
int			parse_cmd_elements(t_cmd *cmd, t_parser *parser);
int			validate_parsed_command(t_cmd *cmd, t_parser *parser);
int			is_builtin_command(char **argv);
void		finalize_command(t_cmd *cmd);

/* parser_pipeline.c */
t_cmd		*parse_pipeline(t_parser *parser);
int			count_pipeline_length(t_cmd *cmd_list);
int			validate_pipeline(t_cmd *cmd_list);

/* parser_redirect.c */
int			parse_redirections(t_parser *parser, t_cmd *cmd);
int			handle_output_redirection(t_cmd *cmd, const char *filename, int append);
int			handle_heredoc_redirection(t_cmd *cmd, const char *delimiter);
int			has_input_redirection(t_cmd *cmd);
int			has_output_redirection(t_cmd *cmd);
void		get_redirection_info(t_cmd *cmd, char *buffer, size_t size);

/* parser_memory.c */
t_cmd		*create_cmd(void);
void		free_cmd(t_cmd *cmd);
void		free_cmd_list(t_cmd *cmd_list);
void		free_argv(char **argv);
char		*duplicate_string(const char *str);
char		**allocate_argv(int argc);
void		cleanup_parser_error(t_parser *parser, t_cmd *partial_cmd);
void		safe_cmd_append(t_cmd **head, t_cmd *new_cmd);

/* test_main.c */
void		test_single_case(const char *input, int should_succeed);
void		run_all_tests(void);
void		interactive_mode(void);
void		print_usage(const char *program_name);

/* ============================================================================
 * LEXER FUNCTION DECLARATIONS (your existing ones)
 * ============================================================================ */

t_token		*tokenize_input(const char *input);
void		print_tokens(t_token *head);
void		free_tokens(t_token *head);
t_token		*create_token(void);
void		add_token(t_token **head, t_token *new_token);
const char	*token_type_to_str(t_token_type type);

/* Internal helper functions */
void		skip_spaces(const char **ptr);
int			is_operator(char c);
t_token		*parse_operator(const char **ptr);
t_token		*parse_word(const char **ptr);
char		*strip_quotes(const char *input);
void		handle_double_quote(t_parse_vars *v);

/* signals */
void		init_signals(void);

#endif 