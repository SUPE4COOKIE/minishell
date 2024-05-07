# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/07 11:06:53 by scrumier          #+#    #+#              #
#    Updated: 2024/05/07 13:41:02 by scrumier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = minishell
SRC_PARSE	= $(wildcard src/parsing/*.c)
SRC_EXEC	= $(wildcard src/exec/*.c)
SRC_BUILDIN	= $(wildcard src/buildin/*.c)
SRC			= $(wildcard src/*.c) $(SRC_PARSE) $(SRC_EXEC) $(SRC_BUILDIN)
OBJ_DIR = .minishell_obj
OBJ     = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CC      = cc
FLAGS   = -Wall -Wextra -Werror -Iincludes -MMD -MP
RM      = rm -rf
LIB		= -Llibft

LIBFT_DIR = libft
LIBFT_MAKEFILE = $(LIBFT_DIR)/Makefile
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_INC = -I$(LIBFT_DIR)/includes
LIBFT_LINK = -L$(LIBFT_DIR) -lft

all: $(NAME)

exec: $(OBJ_EXEC)
	$(CC) $(FLAGS) $(LIB) -o $(NAME) $(OBJ_EXEC)

buildin: $(OBJ_BUILDIN)
	$(CC) $(FLAGS) $(LIB) -o $(NAME) $(OBJ_BUILDIN)

parse: $(OBJ_PARSE)
	$(CC) $(FLAGS) $(LIB) -o $(NAME) $(OBJ_PARSE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile | $(OBJ_DIR)
	$(CC) $(FLAGS) $(LIB) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

libft:
	$(MAKE) -C $(LIBFT_DIR)

-include $(OBJ:.o=.d)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) -o $@ $^

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re