/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:57:22 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/12 15:01:34 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

void	exec(t_minishell *mshell);
bool	is_builtin(char *cmd);
void	replace_hdoc(t_cmd *cmd);
bool	is_builtin(char *cmd);
void	exec_builtin(t_minishell *mshell, t_cmd *cmd);
void	handle_file_redirection(t_minishell *mshell, t_cmd *cmd, int old[2], int new[2]);

#endif