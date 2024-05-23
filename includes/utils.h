/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sonamcrumiere <sonamcrumiere@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 13:15:47 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/23 18:01:10 by sonamcrumie      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

int	error_pipe(char *msg, int new[2], int old[2], t_cmd *cmd);
int	error_cmd(t_minishell *mshell, int status, char *msg);
int	error_msg_free(char *msg, void *ptr, void *ptr2, void *ptr3);

#endif