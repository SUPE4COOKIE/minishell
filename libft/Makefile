# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/30 14:25:42 by scrumier          #+#    #+#              #
#    Updated: 2024/03/25 17:04:52 by scrumier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBFT_SRC_DIR	=	libft_src/
GNL_SRC_DIR		=	get_next_line/
PRINTF_SRC_DIR  =	ft_printf/

LIBFT_SRC		=	$(addprefix $(LIBFT_SRC_DIR), \
					ft_isalnum.c ft_isprint.c ft_memcmp.c ft_putchar_fd.c \
					ft_split.c ft_strlcat.c ft_strncmp.c ft_substr.c \
					ft_isalpha.c ft_itoa.c ft_memcpy.c ft_putendl_fd.c \
					ft_strchr.c ft_strlcpy.c ft_strnstr.c ft_tolower.c \
					ft_bzero.c ft_isascii.c ft_memcpy.c ft_memmove.c \
					ft_putnbr_fd.c ft_strdup.c ft_strlen.c ft_strrchr.c \
					ft_toupper.c ft_calloc.c ft_isdigit.c ft_memchr.c \
					ft_memset.c ft_putstr_fd.c ft_strjoin.c ft_strmapi.c \
					ft_strtrim.c ft_striteri.c ft_atoi.c)

GNL_SRC			=	$(addprefix $(GNL_SRC_DIR), \
					get_next_line.c get_next_line_utils.c)

PRINTF_SRC		=	$(addprefix $(PRINTF_SRC_DIR), \
					ft_printf.c ft_print_uint.c ft_print_str.c \
					ft_print_ptr.c ft_print_int.c ft_print_hex.c)

LIBFT_OBJ_DIR	=	.libft_obj/
GNL_OBJ_DIR		=	.gnl_obj/
PRINTF_OBJ_DIR	=	.printf_obj/

LIBFT_OBJ = $(addprefix $(LIBFT_OBJ_DIR), $(LIBFT_SRC:$(LIBFT_SRC_DIR)%.c=%.o))
GNL_OBJ = $(addprefix $(GNL_OBJ_DIR), $(GNL_SRC:$(GNL_SRC_DIR)%.c=%.o))
PRINTF_OBJ = $(addprefix $(PRINTF_OBJ_DIR), $(PRINTF_SRC:$(PRINTF_SRC_DIR)%.c=%.o))

LIBFT_CC = gcc
LIBFT_FLAGS = -Wall -Wextra -Werror

LIBFT_NAME = libft.a

LIBFT_RM = rm -rf

all: $(LIBFT_NAME)

$(LIBFT_OBJ_DIR)%.o: $(LIBFT_SRC_DIR)%.c | $(LIBFT_OBJ_DIR) Makefile
	$(LIBFT_CC) $(LIBFT_FLAGS) -MMD -MP -c $< -o $@

$(GNL_OBJ_DIR)%.o: $(GNL_SRC_DIR)%.c | $(GNL_OBJ_DIR) Makefile
	$(LIBFT_CC) $(LIBFT_FLAGS) -MMD -MP -c $< -o $@

$(PRINTF_OBJ_DIR)%.o: $(PRINTF_SRC_DIR)%.c | $(PRINTF_OBJ_DIR) Makefile
	$(LIBFT_CC) $(LIBFT_FLAGS) -MMD -MP -c $< -o $@

$(LIBFT_OBJ_DIR) $(GNL_OBJ_DIR) $(PRINTF_OBJ_DIR):
	mkdir -p $@

-include $(LIBFT_OBJ:.o=.d)
-include $(GNL_OBJ:.o=.d)
-include $(PRINTF_OBJ:.o=.d)

$(LIBFT_NAME): $(LIBFT_OBJ) $(GNL_OBJ) $(PRINTF_OBJ) Makefile
	ar rc $@ $^
	ranlib $@

clean:
	$(LIBFT_RM) $(LIBFT_OBJ) $(GNL_OBJ) $(PRINTF_OBJ)

fclean: clean
	$(LIBFT_RM) $(LIBFT_NAME)
	$(LIBFT_RM) $(LIBFT_OBJ_DIR) $(GNL_OBJ_DIR) $(PRINTF_OBJ_DIR)

re: fclean all

.PHONY: all clean fclean re
