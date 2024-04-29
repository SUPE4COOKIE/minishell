CFLAGS = -Wall -Wextra -Werror -MMD -MP
SRC = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
      ft_isdigit.c ft_isprint.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c \
      ft_memset.c ft_strchr.c ft_strdup.c ft_strlcat.c ft_strlcpy.c ft_strlen.c \
      ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_tolower.c ft_toupper.c ft_split.c \
      ft_itoa.c ft_putchar_fd.c ft_putendl_fd.c ft_putstr_fd.c ft_striteri.c ft_strjoin.c \
      ft_strjoin.c ft_strmapi.c ft_strtrim.c ft_substr.c ft_putnbr_fd.c
OBJ = $(SRC:.c=.o)
BONUSSRC = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c \
			ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c \
			ft_lstmap_bonus.c
BONUSOBJ = $(BONUSSRC:.c=.o)
NAME = libft.a
OBJDEPS = $(OBJ:.o=.d)
BONUSOBJDEPS = $(BONUSOBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ) Makefile
	ar -crs $(NAME) $(OBJ)

-include $(BONUSOBJDEPS) $(OBJDEPS)

bonus: $(BONUSOBJ) $(NAME)
	ar -crs $(NAME) $(BONUSOBJ)

%.o: %.c Makefile
	cc $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJ) $(BONUSOBJ) $(BONUSOBJDEPS) $(OBJDEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
