/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:57:22 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/06 16:59:15 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

void exec(t_minishell *mshell);
bool is_builtin(char *cmd);
void replace_hdoc(t_cmd *cmd);

#endif