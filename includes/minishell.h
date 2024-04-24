/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:18:24 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/04/24 17:53:02 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <sys/types.h>

typedef struct s_minishell
{
	char	**env;
	char	**path;
	char	*line;
	char	**history;
	char	*current_path;
	t_cmd	*cmd;
}				t_minishell;

typedef struct s_cmd
{
	char			*cmd;
	pid_t			pid;
	unsigned char	type;
	t_cmd			*next;
}				t_cmd;

execve(cmd->cmd, cmd->args, minishell->env);
#endif