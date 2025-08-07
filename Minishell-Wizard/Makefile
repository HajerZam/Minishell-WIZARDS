# **************************************************************************** #
#                                MINISHELL MAKEFILE                            #
# **************************************************************************** #

NAME        := minishell
CC          := cc
CFLAGS      := -Wall -Wextra -Werror -g
RM          := rm -rf

SRC_DIRS    := . lexer signals parser
OBJ_DIR     := obj
LIBFT_DIR   := libft
INCLUDE_DIR := include

LIBFT       := $(LIBFT_DIR)/libft.a
HEADERS     := -I. -I$(INCLUDE_DIR)

SRC         := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
OBJ         := $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRC)))
SRC_TO_OBJ_MAP = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

all: $(OBJ_DIR) $(LIBFT) $(NAME)

$(NAME): $(SRC_TO_OBJ_MAP)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(SRC_TO_OBJ_MAP) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

$(OBJ_DIR):
	@mkdir -p $@

define MAKE_OBJ_RULE
$(OBJ_DIR)/$(notdir $(1:.c=.o)): $(1)
	@echo "Compiling $(1)..."
	@$(CC) $(CFLAGS) $(HEADERS) -c $$< -o $$@
endef

$(foreach src, $(SRC), $(eval $(call MAKE_OBJ_RULE,$(src))))

$(LIBFT):
	@echo "Building libft..."
	@$(MAKE) -s -C $(LIBFT_DIR)

clean:
	@echo "Cleaning minishell objects..."
	@$(RM) $(OBJ_DIR)
	@$(MAKE) -s -C $(LIBFT_DIR) clean

fclean: clean
	@echo "Removing $(NAME)..."
	@$(RM) $(NAME)
	@$(MAKE) -s -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re