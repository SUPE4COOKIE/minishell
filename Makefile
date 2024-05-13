# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sonamcrumiere <sonamcrumiere@student.42    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/07 11:06:53 by scrumier          #+#    #+#              #
#    Updated: 2024/05/09 12:51:09 by sonamcrumie      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Makefile for compiling the libft library and the files inside src/exec/

# Compiler
CC = cc
# Compiler flags
CFLAGS = -Wall -Wextra -Iincludes

# Directories
LIBFT_DIR = libft
EXEC_DIR = src/exec

# Target executable name
TARGET = minishell

# Source files
SRCS = $(wildcard $(EXEC_DIR)/*.c) $(wildcard src/*.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Include directories
INC_DIRS = -I$(LIBFT_DIR) -I$(EXEC_DIR) -Iincludes

# Phony targets
.PHONY: all clean fclean re

all: $(TARGET)

# Rule to compile the libft library
$(LIBFT_DIR)/libft.a:
	@$(MAKE) -C $(LIBFT_DIR)

# Rule to compile the executable
$(TARGET): $(LIBFT_DIR)/libft.a $(OBJS)
	$(CC) $(CFLAGS) $(INC_DIRS) -o $@ $(OBJS) -L$(LIBFT_DIR) -lft

# Rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -f $(OBJS)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(TARGET)

re: fclean all
