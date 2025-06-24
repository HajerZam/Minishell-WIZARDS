/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:57:49 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/24 13:57:49 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>

typedef enum e_token_type {
    WORD,
    PIPE,           // |
    REDIR_IN,       // <
    REDIR_OUT,      // >
    REDIR_APPEND,   // >>
    HEREDOC         // <<
} t_token_type;

typedef struct s_token {
    char *value;
    t_token_type type;
    struct s_token *next;
} t_token;

typedef enum e_lexer_state {
    STATE_GENERAL,
    STATE_IN_SINGLE_QUOTE,
    STATE_IN_DOUBLE_QUOTE
} t_lexer_state;

// lexer/tokenizer
// Tokenizing functions
t_token     *tokenize_input(const char *input);
void        print_tokens(t_token *head);
void        free_tokens(t_token *head);

// Internal helper functions
void        skip_spaces(const char **ptr);
int         is_operator(char c);
t_token     *parse_operator(const char **ptr);
t_token     *parse_word(const char **ptr);

// signals
void init_signals(void);

#endif