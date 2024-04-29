/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:18:24 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/04/29 05:46:04 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_cmd	t_cmd;

typedef struct s_minishell
{
	char	**env;
	char	**path;
	char	*line;
	char	*infile;
	char	**outfile;
	char	*current_path;
	t_cmd	*cmd;
}				t_minishell;

struct s_cmd
{
	char			*cmd;
	pid_t			pid;
	unsigned char	type;
	t_cmd			*next;
};
int		parse(t_minishell *mshell);
char	**ft_split_args(char const *str, char sep);
char	*copy_str(const char *str, size_t start, size_t len);
int		is_in_quote(char c, char *in_quote);
void	free_str(char **split);
char	**init_split(size_t count);
#endif