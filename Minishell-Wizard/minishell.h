/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:57:49 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/30 15:50:11 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <dirent.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "include/libft.h"

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC
}		t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}		t_token;

typedef enum e_lexer_state
{
	STATE_GENERAL,
	STATE_IN_SINGLE_QUOTE,
	STATE_IN_DOUBLE_QUOTE
}		t_lexer_state;

typedef struct s_parse_vars
{
	const char		*input;
	t_token			*token;
	char			*buffer;
	size_t			i;
	size_t			j;
	t_lexer_state	state;
}		t_parse_vars;

typedef struct s_cmd
{
	char			**argv;
	int				input_fd;
	int				output_fd;
	struct s_cmd	*next;
	struct s_cmd	*tokens;
	int				heredoc_fd;
	int				is_heredoc;
	int				is_builtin;
	char			**envp;
	int				exit_status;
}		t_cmd;

typedef struct s_parser
{
	t_token			*tokens;
	t_token			*current;
	t_cmd			*cmd_list;
	int				error;
}		t_parser;

typedef struct s_env
{
	char			**envp;
	char			*key;
	char			*value;
	int				is_exported;
	struct s_env	*next;
}		t_env;

typedef struct s_var_data
{
	char	*result;
	t_env	*env;
	int		last_exit_status;
	size_t	*i;
	size_t	*start;
}		t_var_data;

/* Execution context structure */
typedef struct s_exec_context
{
	char	**envp;
	t_env	*env;
	t_env	*exported_env;
	int		last_exit_status;
	int		pipe_count;
	int		**pipes;
	pid_t	*pids;
	int		stdin_backup;
	int		stdout_backup;
	int		in_main_loop;
}		t_exec_context;

/* Global variable to store last exit status */
extern volatile sig_atomic_t	g_signal_received;

/* lexer/tokenization */

t_token		*tokenize_input(const char *input);
int			has_unclosed_quotes(const char *input);
void		print_tokens(t_token *head);
void		free_tokens(t_token *head);
t_token		*create_token(void);
void		add_token(t_token **head, t_token *new_token);
const char	*token_type_to_str(t_token_type type);
char		*get_complete_input(void);

/* internal utility functions */
void		skip_spaces(const char **ptr);
int			is_operator(char c);
t_token		*parse_operator(const char **ptr);
t_token		*parse_word(const char **ptr);
char		*strip_quotes(const char *input);
void		handle_double_quote(t_parse_vars *v);

/* signal handling */
void		init_signals(void);
void		init_signals_interactive(void);
void		init_signals_execution(void);
void		init_signals_child(void);
void		handle_sigint_interactive(int sig);
void		handle_sigint_execution(int sig);
int			wait_for_processes_with_signals(t_exec_context *ctx);

/* parser_main.c */
t_cmd		*parse_command_line(t_token *tokens);
void		init_parser(t_parser *parser, t_token *tokens);
int			validate_syntax(t_token *tokens);

/* parser_utils.c */
t_token		*peek_token(t_parser *parser);
t_token		*consume_token(t_parser *parser);
int			expect_token(t_parser *parser, t_token_type type);
int			is_redirection(t_token_type type);
int			handle_word_token(t_cmd *cmd, t_parser *parser, int *arg_index,
				int arg_count);
int			count_words_until_pipe(t_parser *parser);
void		print_cmd_tree(t_cmd *cmd, int depth);
void		print_execution_tree(t_cmd *cmd_list);
void		print_pipeline_summary(t_cmd *cmd_list);

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
int			handle_input_redirection(t_cmd *cmd, const char *filename);
int			handle_output_redirection(t_cmd *cmd, const char *filename,
				int append);
int			handle_heredoc_redirection(t_cmd *cmd, const char *delimiter);
int			has_input_redirection(t_cmd *cmd);
int			has_output_redirection(t_cmd *cmd);
int			heredoc_check(t_cmd *cmd, const char *delimiter, int pipe_fd[2]);
void		get_redirection_info(t_cmd *cmd, char *buffer, size_t size);
void		heredoc_cleanup(t_cmd *cmd, int pipe_fd[2]);

/* parser_memory.c */
t_cmd		*create_cmd(void);
void		free_cmd(t_cmd *cmd);
void		free_cmd_list(t_cmd *cmd_list);
void		free_argv(char **argv);
char		*duplicate_string(const char *str);
char		**allocate_argv(int argc);
void		cleanup_parser_error(t_parser *parser, t_cmd *partial_cmd);
void		safe_cmd_append(t_cmd **head, t_cmd *new_cmd);

/* Export utility functions */
int			parse_assignment(const char *arg, char **key, char **value);
t_env		*find_env_var(t_env *env, const char *key);
int			add_env_var_exported(t_env **env, const char *key,
				const char *value);
int			export_with_assignment(const char *key, const char *value,
				t_env **env);
int			export_existing_var(const char *key, t_env **env);
char		*extract_var_value(const char *str);
char		*extract_var_name(const char *str);
int			handle_export_assignment(t_env **env, const char *arg);
int			handle_export_simple(t_env **env, const char *arg);

/* builtins */
int			builtin_cd(char **argv, t_env *env);
int			builtin_echo(char **argv);
int			builtin_pwd(void);
int			builtin_exit(char **argv);
int			builtin_env(t_env *env);
int			builtin_export(char **argv, t_env **env);
int			builtin_unset(char **argv, t_env **env);
int			setup_builtin_fds(t_cmd *cmd, int *saved_stdin,
				int *saved_stdout);
void		restore_builtin_fds(int saved_stdin, int saved_stdout);
int			handle_builtin(t_cmd *cmd, t_env **env);
int			execute_builtin(t_cmd *cmd, t_env **env);
int			is_builtin(const char *cmd);

/* CD builtin functions */
int			update_env_variables(t_env *env, const char *old_pwd);
int			update_pwd_env(t_env *env, const char *new_pwd);
int			update_olpwd_env(t_env *env, const char *old_pwd);
char		*get_current_pwd(void);
char		*resolve_cd_target(char **argv, t_env *env);
int			update_working_directory(const char *target_pwd);
char		*handle_cd_home(t_env *env);
char		*handle_cd_oldpwd(t_env *env);
char		*handle_cd_path(const char *path, t_env *env);
char		*expand_tilde(const char *path, t_env *env);
int			validate_cd_args(char **argv);
void		print_cd_error(const char *path, const char *error_message);

/*env management*/

int			ft_strcmp(const char *s1, const char *s2);
t_env		*init_env(char **envp);
t_env		*create_env_copy_direct(char **envp);
void		free_env(t_env *env);
char		*add_text_before_var(const char *input, t_var_data *data);
char		*process_valid_variable(const char *input, t_var_data *data,
				size_t var_len);
char		*process_variable(const char *input, t_var_data *data);
char		*add_remaining_text(const char *input, size_t i,
				size_t start, char *result);
char		*getenv_from_list(t_env *env, const char *key);
int			update_env_value(t_env **env, const char *key,
				const char *value);
int			add_env_var(t_env **env, const char *key, const char *value);
char		*expand_variables(const char *input,
				t_env *env, int last_exit_status);
char		*expand_variable(const char *input,
				t_env *env, int last_exit_status);
char		*get_variable_value(const char *var_name,
				size_t var_len, t_env *env, int last_exit_status);
size_t		get_var_name_len(const char *str);
int			should_expand_variable(const char *input, size_t i);
int			is_in_single_quotes(const char *str, size_t pos);
char		**env_list_to_array(t_env *env);
char		*find_command_path_env(char *command, t_env *env);
void		free_env_array(char **envp, int count);

/* Main execution functions */
void		init_exec_context_struct(t_exec_context *ctx);
int			allocate_pipe_arrays(t_exec_context *ctx, int cmd_count);
void		initialize_pipes_array(t_exec_context *ctx, int cmd_count);
int			create_pipe_fds(t_exec_context *ctx, int cmd_count);
int			execute_pipeline(t_cmd *cmd_list, t_exec_context *ctx);
int			execute_single_command(t_cmd *cmd, t_exec_context *ctx,
				int cmd_index, t_cmd *cmd_list);
int			execute_external(t_cmd *cmd, t_exec_context *ctx, int cmd_index, t_cmd *cmd_list);
int			execute_single_command_no_pipe(t_cmd *cmd, t_exec_context *ctx);
int			execute_external_single(t_cmd *cmd, t_exec_context *ctx);

/* Pipeline management */
int			is_directory(char *path);
int			setup_pipe_fds(t_cmd *cmd, t_exec_context *ctx, int cmd_index);
void		cleanup_pipes(t_exec_context *ctx);
int			setup_pipeline(t_cmd *cmd_list, t_exec_context *ctx);
int			execute_pipeline_commands(t_cmd *cmd_list, t_exec_context *ctx);
void		handle_child_process(t_cmd *cmd, t_exec_context *ctx, t_cmd *cmd_list);
void		handle_parent_process(pid_t pid, t_exec_context *ctx);
void		handle_pipeline_child(t_cmd *cmd, t_exec_context *ctx, 
						int cmd_index, t_cmd *cmd_list);
void		free_ctx_list(t_exec_context *ctx, t_cmd *cmd_list);
void		cleanup_child_process(t_exec_context *ctx, t_cmd *cmd_list);
int			setup_input_pipe(t_cmd *cmd, t_exec_context *ctx, int cmd_index);
int			setup_output_pipe(t_cmd *cmd, t_exec_context *ctx,
				int cmd_index);
void		close_all_pipe_fds(t_exec_context *ctx);

int			setup_pipes(t_exec_context *ctx, int cmd_count);
int			is_directory(char *path);
int			setup_pipe_fds(t_cmd *cmd, t_exec_context *ctx, int cmd_index);

/* Redirection handling */
int			setup_redirections(t_cmd *cmd);
int			restore_std_fds(t_exec_context *ctx);
int			backup_std_fds(t_exec_context *ctx);

/* Process management */
int			wait_for_processes(t_exec_context *ctx);
void		handle_signals_in_child(void);
void		handle_signals_in_parent(void);

/* Path resolution */
char		*find_command_path(char *command);
char		*find_command_path_env(char *command, t_env *env);
int			is_executable_file(char *path);

/* Utility functions */
int			count_commands(t_cmd *cmd_list);
void		cleanup_execution_context(t_exec_context *ctx);
int			init_execution_context(t_exec_context *ctx, char **envp);
void		print_execution_error(char *command, char *error);

/* Main utility functions */
void		cleanup_resources(t_exec_context *ctx, t_env *env);
void		cleanup_and_exit(t_exec_context *ctx, t_env *env, int exit_status);
int			process_command_line(char *input, t_exec_context *ctx);
int			setup_execution_context(t_exec_context *ctx, t_env *env,
				char **envp);
int			handle_exit_command(t_cmd *cmd_list, t_exec_context *ctx);
int			process_tokens(char *expanded_input, t_exec_context *ctx);
void		handle_readline_interruption(t_exec_context *ctx);
int			init_shell(t_exec_context *ctx, t_env **env, char **envp);
int			process_input_line(char *input, t_exec_context *ctx);

#endif