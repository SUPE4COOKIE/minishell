/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sonamcrumiere <sonamcrumiere@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:18:24 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/09 14:38:14 by sonamcrumie      ###   ########.fr       */
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

typedef struct s_cmd	t_cmd;

/*
* @breif: Structure to store the minishell data
* @param env: The environment variables
* @param path: The PATH variable
* @param line: The command line
* @param current_path: The current path
* @param last_exit_status: The last exit status
* @param cmd: The command structure
*/
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

#endif