/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:18:24 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/20 00:03:19 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>

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
	T_PATH,
}				t_token_type;

typedef struct s_minishell
{
	char	**env;
	char	**path;
	char	*line;
	char	*current_path;
	int		last_exit_status;
	t_cmd	*cmd;
}				t_minishell;

typedef enum e_cmd_type
{
	CMD,
	PIP,
	RED_IN,
	RED_OUT,
	HDOC,
	APP_OUT,
}				t_cmd_type;

struct s_cmd
{
	char			*cmd;
	char			**args;
	int				pipe;
	pid_t			pid;
	t_cmd_type		type;
	char			*infile;
	char			*outfile;
	t_cmd			*prev;
	t_cmd			*next;
};

struct s_lexer
{
	char			*value;
	bool			space_after;
	t_token_type	type;
	t_lexer			*next;
	t_lexer			*prev;
};

int		parse(t_minishell *mshell);
char	**ft_split_args(char const *str, char sep);
char	*copy_str(const char *str, size_t start, size_t len);
int		is_in_quote(char c, char *in_quote);
void	free_str(char **split);
char	**init_split(size_t count);
t_lexer	*lexer(char *line);
void	print_lexer(t_lexer *lex);
int		validate(t_lexer *lex);
int	new_lexer(t_lexer **lex, char *line, size_t size);
int	save_path(t_minishell *mshell, char **env);
t_cmd	*lexer_to_cmd(t_lexer *lex);
t_lexer	*get_last_lexer(t_lexer *lex);
# include "utils.h"
# include "exec.h"

#endif