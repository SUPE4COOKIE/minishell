/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:18:24 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/07 17:47:50 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_cmd	t_cmd;
typedef struct s_lexer	t_lexer;

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
	PIPE,
	RED_IN,
	RED_OUT,
	APP_IN,
	APP_OUT,
}				t_cmd_type;

struct s_cmd
{
	char			*cmd;
	pid_t			pid;
	unsigned char	type;
	char			*infile;
	char			*outfile;
	t_cmd			*next;
};

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
}				t_token_type;

struct s_lexer
{
	char			*line;
	t_token_type	type;
	t_lexer			*next;
};

int		parse(t_minishell *mshell);
char	**ft_split_args(char const *str, char sep);
char	*copy_str(const char *str, size_t start, size_t len);
int		is_in_quote(char c, char *in_quote);
void	free_str(char **split);
char	**init_split(size_t count);

#endif