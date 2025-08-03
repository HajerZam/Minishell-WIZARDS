/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:57:49 by halzamma          #+#    #+#             */
/*   Updated: 2025/08/03 15:16:17 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "include/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>

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

typedef struct s_cmd
{
    char			**argv;
    int				input_fd;
    int				output_fd;
    struct s_cmd	*next;
    int				is_builtin;
}	t_cmd;

typedef struct s_env
{
    char			*key;
    char			*value;
    struct s_env	*next;
}	t_env;

typedef struct s_var_data
{
	size_t	*i;
	size_t	*start;
	t_env	*env;
	char	*result;
}	t_var_data;

// Global variable for exit status
int g_last_exit_status = 0;

// lexer/tokenizer

t_token		*tokenize_input(const char *input);
void		print_tokens(t_token *head);
void		free_tokens(t_token *head);
t_token		*create_token(void);
void		add_token(t_token **head, t_token *new_token);
const char	*token_type_to_str(t_token_type type);

// Internal helper functions
void		skip_spaces(const char **ptr);
int			is_operator(char c);
t_token		*parse_operator(const char **ptr);
t_token		*parse_word(const char **ptr);
char		*strip_quotes(const char *input);
void		handle_double_quote(t_parse_vars *v);

//	signals 
void		init_signals(void);

//	builtins

int 		builtin_cd(char **argv, t_env *env);
int			builtin_echo(char **argv);
int 		builtin_pwd(void);
int			builtin_exit(char **argv);
int 		builtin_env(t_env *env);
int			builtin_export(char **argv, t_env *env);
int			builtin_unset(char **argv, t_env *env);
int			handle_builtin(t_cmd *cmd, t_env *env);

// env management

t_env		init_env(char **envp);
void		free_env(t_env *env);
char		*getenv_from_list(t_env *env, const char *key);
int			update_env_value(t_env **env, const char *key, const char *value);
char		*expand_variables(const char *input, t_env *env, int last_exit_status);
int			add_env_var(t_env **env, const char *key, const char *value);


#endif
