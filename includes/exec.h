/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:57:22 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/17 18:28:16 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# define TMP_FILE_PREFIX "/tmp/minishell_hdoc_"
# define URANDOM_PATH "/dev/urandom"
# define RANDOM_BYTES 8

void	exec(t_minishell *mshell);
bool	is_builtin(char *cmd);
void	replace_hdoc(t_cmd *cmd, int old[2], int new[2]);
bool	is_builtin(char *cmd);
void	exec_builtin(t_minishell *mshell, t_cmd *cmd);
void	handle_file_redirection(t_cmd *cmd, int old[2], int new[2]);
void	handle_hdoc(t_cmd *cmd, int old[2], int new[2], char *tmp_filename);
void	ft_close(int old[2], int new[2]);
void	init_exec(int old[2], int new[2], t_minishell *mshell);
void	close_old(int i, int old[2]);
void	fork_exec(t_minishell *mshell, int old[2], int new[2], int i);
void	dup_cmd(int i, t_cmd *cmd, int old[2], int new[2]);
void	exec_cmd(t_minishell *mshell, t_cmd *cmd);
void	generate_unique_filename(char *buffer, size_t length);
void	signal_exec(int signal);

#endif