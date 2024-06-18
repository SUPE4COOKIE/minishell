NAME = minishell

# Source files
SRCS = $(addprefix src/, \
	$(addprefix builtin/, \
		cd.c \
		echo.c \
		env.c \
		exit.c \
		export.c \
		pwd.c \
		unset.c \
	) \
	$(addprefix exec/, \
		exec.c \
		handle_builtins.c \
		handle_redir.c \
		fork_exec.c \
		handle_hdoc.c \
	) \
	$(addprefix parsing/, \
		$(addprefix lexer/, \
			lexer.c \
			validator.c \
		) \
		$(addprefix expand/, \
			expand.c \
		) \
		cmd_finder.c \
		env_parse.c \
		files_validator.c \
		ft_split_args_utils.c \
		ft_split_args.c \
		lexer_to_cmd.c \
		main_parser.c \
		pre_parse.c \
	) \
	$(addprefix utils/, \
		error.c \
		utils.c \
		name_file.c \
		fork.c \
		pid_utils.c \
		env.c \
		list.c \
		remove_double_points.c \
		change_dir.c \
	) \
	minishell.c \
	)

# Object files directory and object files
OBJ_DIR = .obj
OBJ = $(SRCS:src/%.c=$(OBJ_DIR)/%.o)

# Compiler and flags
CC = cc
FLAGS = -Wall -Wextra -Werror -Iincludes -Ilibft -g3

# Libraries
LIB = -Llibft -lft -lreadline
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Clean up command
RM = rm -rf

# Targets
all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/parsing
	mkdir -p $(OBJ_DIR)/parsing/lexer
	mkdir -p $(OBJ_DIR)/parsing/expand
	mkdir -p $(OBJ_DIR)/builtin
	mkdir -p $(OBJ_DIR)/exec
	mkdir -p $(OBJ_DIR)/utils

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) $(LIB) -o $@

clean:
	$(RM) $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all