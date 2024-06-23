/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 13:15:47 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/23 22:28:45 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

int	error_pipe(char *msg, int new[2], int old[2], t_cmd *cmd);
int	error_cmd(t_minishell *mshell, int status, char *msg);
int	error_msg_free(char *msg, void *ptr, void *ptr2, void *ptr3);
long ft_atoi(const char *str);
char *tmp_file(int nb);
int free_tab(char **tab);
void print_tab(char **tab);
bool	is_redir_before(t_cmd *cmd, char **redir, char **reference);
void	free_mshell(t_minishell *mshell);
#endif