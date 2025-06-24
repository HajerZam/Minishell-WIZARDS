# **************************************************************************** #
#                                MINISHELL MAKEFILE                            #
# **************************************************************************** #

NAME        := minishell
CC          := cc
CFLAGS      := -Wall -Wextra -Werror -g
RM          := rm -rf

# Directories
SRC_DIRS    := . lexer signals
OBJ_DIR     := obj
LIBFT_DIR   := libft
INCLUDE_DIR := include

# Libft
LIBFT       := $(LIBFT_DIR)/libft.a
LIBFT_INC   := -I$(INCLUDE_DIR)

# Headers
HEADERS     := -I. -I$(INCLUDE_DIR)

# Collect all .c sources from subdirs
SRC         := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))

# Map full src path to obj path: e.g., lexer/lexer.c → obj/lexer.o
OBJ         := $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRC)))

# Map .c file to corresponding .o manually using a pattern rule
SRC_TO_OBJ_MAP = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

# Default target
all: $(OBJ_DIR) $(LIBFT) $(NAME)

$(NAME): $(SRC_TO_OBJ_MAP)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(SRC_TO_OBJ_MAP) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

# Ensure obj directory exists
$(OBJ_DIR):
	@mkdir -p $@

# Build each object file individually from known source file
define MAKE_OBJ_RULE
$(OBJ_DIR)/$(notdir $(1:.c=.o)): $(1)
	@echo "Compiling $(1)..."
	@$(CC) $(CFLAGS) $(HEADERS) -c $$< -o $$@
endef

$(foreach src, $(SRC), $(eval $(call MAKE_OBJ_RULE,$(src))))

# Libft rule
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

# Cleanup
clean:
	@$(RM) $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re