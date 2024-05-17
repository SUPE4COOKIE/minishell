/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:18:24 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/17 15:15:58 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <errno.h>
# define PATH_MAX 4096

typedef struct s_cmd	t_cmd;

typedef struct s_minishell
{
	char	**env;
	char	**path;
	char	*line;
	char	*current_path;
	int		last_exit_status;
	char 	*pwd;
	char	*old_pwd;
	t_cmd	*cmd;
}				t_minishell;

typedef enum e_cmd_type
{
	CMD,
	PIP,
	RED_IN,
	RED_OUT,
	APP_IN,
	APP_OUT,
}				t_cmd_type;

struct s_cmd
{
	char			**cmd;
	int				pipe;
	pid_t			pid;
	unsigned char	type;
	char			*infile;
	char			*outfile;
	t_cmd			*prev;
	t_cmd			*next;
};

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
}				t_token_type;

typedef struct s_lexer
{
	char			*line;
	t_token_type	type;
}				t_lexer;

# include "exec.h"
# include "builtins.h"
# include "utils.h"

#endif