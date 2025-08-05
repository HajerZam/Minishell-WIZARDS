/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:57:49 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/05 14:46:29 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
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
	int				is_builtin;
}		t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
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

/* Global variable to store last exit status */
extern int	g_last_exit_status;

/* lexer/tokenization */

t_token		*tokenize_input(const char *input);
void		print_tokens(t_token *head);
void		free_tokens(t_token *head);
t_token		*create_token(void);
void		add_token(t_token **head, t_token *new_token);
const char	*token_type_to_str(t_token_type type);

/* internal utility functions */
void		skip_spaces(const char **ptr);
int			is_operator(char c);
t_token		*parse_operator(const char **ptr);
t_token		*parse_word(const char **ptr);
char		*strip_quotes(const char *input);
void		handle_double_quote(t_parse_vars *v);

/* signal handling */
void		init_signals(void);

/* builtins */

int			builtin_cd(char **argv, t_env *env);
int			builtin_echo(char **argv);
int			builtin_pwd(void);
int			builtin_exit(char **argv);
int			builtin_env(t_env *env);
int			builtin_export(char **argv, t_env *env);
int			builtin_unset(char **argv, t_env *env);
int			handle_builtin(t_cmd *cmd, t_env *env);

/*env management*/

int			ft_strcmp(const char *s1, const char *s2);
t_env		*init_env(char **envp);
void		free_env(t_env *env);
char		*process_valid_variable(const char *input, t_var_data *data,
				size_t var_len);
char		*process_variable(const char *input, t_var_data *data);
char		*add_remaining_text(const char *input, size_t i,
				size_t start, char *result);
char		*getenv_from_list(t_env *env, const char *key);
int			update_env_value(t_env **env, const char *key, const char *value);
int			unset_env_var(t_env **env, const char *key);
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

#endif
