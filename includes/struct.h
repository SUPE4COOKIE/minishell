/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:15:56 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/22 14:17:24 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_cmd			t_cmd;
typedef struct s_lexer			t_lexer;

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
	int		original_stdout;
	int		original_stdin;
	char	*invalid_redir;
	t_cmd	*cmds;
}				t_minishell;

typedef enum e_cmd_type
{
	CMD,
	PIP,
	RED_IN,
	RED_OUT,
	HDOC,
	APP_OUT,
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

#endif