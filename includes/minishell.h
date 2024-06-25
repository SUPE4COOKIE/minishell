/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:18:24 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/25 11:42:19 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <errno.h>
# include <stdint.h>
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif
# define PRINT_CAT false
# define DEBUG true
# define DEFAULT_PATH "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"

extern int g_sig;

typedef struct s_cmd	t_cmd;
typedef struct s_lexer	t_lexer;

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_APPEND_OUT,
	T_HERE_DOC,
	T_D_QUOTED_WORD,
	T_S_QUOTED_WORD,
}				t_token_type;

typedef struct s_minishell
{
	char	**env;
	char	**path;
	char	*line;
	int		last_exit_status;
	bool	in_heredoc;
	int		last_pid;
	t_cmd	*cmds;
}				t_minishell;

typedef enum e_cmd_type
{
	CMD,
	FIL,
	PIP,
	RED_IN, // <
	RED_OUT, // >
	HDOC, // <<
	APP_OUT, // >>
	UNDEFINED,
}				t_cmd_type;

struct s_cmd
{
	char			*cmd;
	bool			is_valid_cmd;
	char			**args;
	t_cmd_type		op_type[2];
	char			**infile;
	char			**outfile;
	t_cmd_type		*type_chain;
	t_cmd			*prev;
	t_cmd			*next;
};

# include "parsing.h"
# include "utils.h"
# include "exec.h"
# include "builtins.h"

#endif