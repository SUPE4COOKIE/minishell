NAME = minishell

# Source files
SRCS = $(addprefix src/, \
	print_cat.c handle-c.c \
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
		name_file.c \
		redir.c \
		check_files.c \
	) \
	$(addprefix parsing/, \
		$(addprefix lexer/, \
			lexer.c \
			type_handling.c \
			word_processing.c \
		) \
		$(addprefix expand/, \
			expand.c \
			expand_str_tools.c \
			lexer_expand.c \
			var_handling.c \
		) \
		$(addprefix syntax/, \
			syntax_checks.c \
		) \
		$(addprefix lexer_to_cmd/, \
			lexer_to_cmd.c \
			redirections.c \
			args.c \
			space_addition.c \
		) \
		cmd_finder.c \
		env_parse.c \
		main_parser.c \
	) \
	$(addprefix utils/, \
		$(addprefix cmd/, \
			cmd_utils.c \
			cmd_delete_utils.c \
			redir_order.c \
		) \
		$(addprefix lexer/, \
			lexer_info_utils.c \
			lexer_str_utils.c \
			lexer_delete_utils.c \
			lexer_modify_utils.c \
		) \
		error.c \
		exit_free.c \
		utils.c \
		cmd_finder_errors.c \
		name_file.c \
		pid_utils.c \
		env_utils.c \
		list.c \
		remove_double_points.c \
		path_utils.c \
		change_dir.c \
		fds.c \
		ascii_to.c \
		exec_utils.c \
		redir_utils.c \
	) \
	minishell.c \
	)

# Object files directory and object files
OBJ_DIR = .obj
OBJ = $(SRCS:src/%.c=$(OBJ_DIR)/%.o)

# Compiler and flags
CC = cc
CFLAGS = -Wall -Werror -Wextra -Iincludes -Ilibft -g3

# Libraries
LIB = -Llibft -lft -lreadline
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Clean up command
RM = rm -rf

# Directory structure
DIRS = $(OBJ_DIR) \
	$(OBJ_DIR)/parsing \
	$(OBJ_DIR)/parsing/lexer \
	$(OBJ_DIR)/parsing/lexer_to_cmd \
	$(OBJ_DIR)/parsing/expand \
	$(OBJ_DIR)/parsing/syntax \
	$(OBJ_DIR)/builtin \
	$(OBJ_DIR)/exec \
	$(OBJ_DIR)/utils \
	$(OBJ_DIR)/utils/cmd \
	$(OBJ_DIR)/utils/lexer

# Header files
HEADERS = $(addprefix includes/, \
		builtins.h \
		exec.h \
		minishell.h \
		parsing.h \
		struct.h \
		utils.h \
		)
LIBFT_HEADERS = $(addprefix libft/, \
		ft_printf.h \
		get_next_line.h \
		libft.h \
		)

# Targets
all: $(LIBFT) $(NAME)

$(LIBFT): $(LIBFT_HEADERS) libft/Makefile
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: src/%.c $(HEADERS) Makefile $(LIBFT) | $(DIRS)
	$(CC) $(CFLAGS) -c $< -o $@

$(DIRS):
	mkdir -p $(DIRS)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $@

clean:
	$(RM) $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
