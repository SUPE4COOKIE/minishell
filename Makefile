NAME = minishell
SRCS = src/minishell.c $(wildcard src/parsing/*.c) $(wildcard src/parsing/lexer/*.c)
OBJ_DIR = .obj
OBJ = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRCS))
CC = cc
FLAGS = -Wall -Wextra -Werror -Iincludes -Ilibft -g3
LIB = -Llibft -lft -lreadline
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
RM = rm -rf

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@
	mkdir -p $(OBJ_DIR)/parsing
	mkdir -p $(OBJ_DIR)/parsing/lexer

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) $(LIB) -o $@

clean:
	$(RM) $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re