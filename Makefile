CC = cc
CFLAGS = -MMD -MP -Wall -Wextra -Werror -g3
NAME = minishell

LIBFT_PATH = libft
LIBFT = $(LIBFT_PATH)/libft.a
LIBFT_SRC_FILES = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
		ft_isdigit.c ft_isprint.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c \
		ft_memset.c ft_strchr.c ft_strdup.c ft_strlcat.c ft_strlcpy.c ft_strlen.c \
		ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_tolower.c ft_toupper.c ft_split.c \
		ft_itoa.c ft_putchar_fd.c ft_putendl_fd.c ft_putstr_fd.c ft_striteri.c ft_strjoin.c \
		ft_strjoin.c ft_strmapi.c ft_strtrim.c ft_substr.c ft_putnbr_fd.c
LIBFT_SRC = $(addprefix $(LIBFT_PATH)/, $(LIBFT_SRC_FILES))
SRC_PATH = src
SRC_FILES = minishell.c
HEADER_PATH = includes
HEADER_FILES = minishell.h
HEADERS = $(addprefix $(HEADER_PATH)/, $(HEADER_FILES))
SRCS = $(addprefix $(SRC_PATH)/, $(SRC_FILES))
OBJ_PATH = .obj
OBJ = $(addprefix $(OBJ_PATH)/, $(SRC_FILES:.c=.o))
OBJDEPS = $(addprefix $(OBJ_PATH)/, $(OBJ:.o=.d))

all: $(LIBFT) $(NAME)

$(NAME): $(OBJ) $(LIBFT) Makefile
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline -L$(LIBFT_PATH) -lft

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c Makefile $(HEADERS) | $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

$(LIBFT): $(LIBFT_SRC)
	make -C $(LIBFT_PATH)

-include $(OBJDEPS)

clean:
	rm -rf $(OBJ_PATH)
	make -C $(LIBFT_PATH) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re